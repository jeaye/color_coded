#pragma once

#include <jest/jest.hpp>

#include "env/environment.hpp"
#include "conf/load.hpp"
#include "conf/defaults.hpp"

namespace color_coded
{
  struct config_test{};
  using config_group = jest::group<config_test>;
  static config_group const config_obj{ "loading and reading of config files" };
}

namespace std
{
  /* Allow the usage of expect_equal, which prints values when they don't match. */
  std::ostream& operator <<(std::ostream &os, std::vector<std::string> const &v)
  {
    os << "{ ";
    std::copy(v.begin(), v.end(), std::ostream_iterator<std::string>(os, ", "));
    os.seekp(-2);
    return os << " }";
  }
}

namespace jest
{
  auto constexpr test_config("test/config/.color_coded");

  template <> template <>
  void color_coded::config_group::test<0>() /* Empty defaults. */
  { expect_equal(color_coded::conf::load(""), color_coded::conf::defaults()); }

  template <> template <>
  void color_coded::config_group::test<1>() /* Non-existent file defaults. */
  { expect_equal(color_coded::conf::load("nonexistent/file"), color_coded::conf::defaults()); }

  template <> template <>
  void color_coded::config_group::test<2>() /* Valid file. */
  {
    auto const args(color_coded::conf::load(test_config));
    expect
    (
      std::find
      (
        args.begin(), args.end(),
        std::string{ "-I" } + color_coded::environment<color_coded::env::tag>::pwd + "/test"
      ) != args.end()
    );
    expect(std::find(args.begin(), args.end(), "-I/etc/") != args.end());
    expect(std::find(args.begin(), args.end(), "foo") != args.end());
    expect_equal
    (
      args.size(),
      3 + color_coded::conf::pre_constants().size() +
          color_coded::conf::post_constants().size()
    );
  }

  template <> template <>
  void color_coded::config_group::test<3>() /* Constants always present. */
  {
    auto const args(color_coded::conf::load(test_config));
    for(auto const &constant : color_coded::conf::pre_constants())
    { expect(std::find(args.begin(), args.end(), constant) != args.end()); }
    for(auto const &constant : color_coded::conf::post_constants())
    { expect(std::find(args.begin(), args.end(), constant) != args.end()); }
  }
}
