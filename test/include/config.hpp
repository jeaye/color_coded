#pragma once

#include <jest/jest.hpp>

#include "env/environment.hpp"
#include "conf/load.hpp"
#include "conf/find.hpp"
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
  auto constexpr test_config_dir("test/config");
  auto constexpr test_config("test/config/.color_coded");
  auto constexpr test_config_typed("test/config/.color_coded_foo");

  template <> template <>
  void color_coded::config_group::test<0>() /* Empty defaults. */
  {
    expect_equal
    (
      color_coded::conf::load("", "c"),
      color_coded::conf::defaults("c")
    );
    expect_equal
    (
      color_coded::conf::load("", "c++"),
      color_coded::conf::defaults("c++")
    );
    expect_equal
    (
      color_coded::conf::load("", "objective-c"),
      color_coded::conf::defaults("objective-c")
    );
    expect_equal
    (
      color_coded::conf::load("", "objective-c++"),
      color_coded::conf::defaults("objective-c++")
    );
  }

  template <> template <>
  void color_coded::config_group::test<1>() /* Non-existent file defaults. */
  {
    expect_equal
    (
      color_coded::conf::load("nonexistent/file", "c"),
      color_coded::conf::defaults("c")
    );
    expect_equal
    (
      color_coded::conf::load("nonexistent/file", "c++"),
      color_coded::conf::defaults("c++")
    );
    expect_equal
    (
      color_coded::conf::load("nonexistent/file", "objective-c"),
      color_coded::conf::defaults("objective-c")
    );
    expect_equal
    (
      color_coded::conf::load("nonexistent/file", "objective-c++"),
      color_coded::conf::defaults("objective-c++")
    );
  }

  template <> template <>
  void color_coded::config_group::test<2>() /* Valid file. */
  {
    auto const args(color_coded::conf::load(test_config, "c++"));
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
      3 + color_coded::conf::pre_constants("c++").size() +
          color_coded::conf::post_constants().size()
    );
  }

  template <> template <>
  void color_coded::config_group::test<3>() /* Constants always present. */
  {
    auto const args(color_coded::conf::load(test_config, "c++"));
    for(auto const &constant : color_coded::conf::pre_constants("c++"))
    { expect(std::find(args.begin(), args.end(), constant) != args.end()); }
    for(auto const &constant : color_coded::conf::post_constants())
    { expect(std::find(args.begin(), args.end(), constant) != args.end()); }
  }

  template <> template <>
  void color_coded::config_group::test<4>() /* Prefer language specific config. */
  {
    auto const file(color_coded::conf::find(test_config, "foo"));
    expect_equal(file, test_config_typed);
  }
}
