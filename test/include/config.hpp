#pragma once

#include <jest/jest.hpp>

#include "core.hpp"

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
  auto constexpr compile_commands("test/config/compile_commands.json");
  auto constexpr test_file("/usr/src/color_coded/myfile.cpp");
  auto constexpr test_header("/usr/src/color_coded/myfile.hpp");
  auto constexpr test_include("/usr/include/color_coded/myfile.hpp");

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
        std::string{ "-I" } +
        color_coded::environment<color_coded::env::tag>::pwd +
        "/" + test_config_dir + "/test"
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

  template <> template <>
  void color_coded::config_group::test<5>() /* compilation database, valid file. */
  {
    auto const args(color_coded::conf::load(compile_commands, "c++", test_file));
    expect(std::find(args.begin(), args.end(), "-I/etc/") != args.end());
    expect(std::find(args.begin(), args.end(), "-foo") != args.end());
    expect(std::find(args.begin(), args.end(), test_file) == args.end());
  }

  template <> template <>
  void color_coded::config_group::test<6>() /* compilation database, header without flags. */
  {
    auto const args_source(color_coded::conf::load(compile_commands, "c++", test_file));
    auto const args_header(color_coded::conf::load(compile_commands, "c++", test_header));
    auto const args_include(color_coded::conf::load(compile_commands, "c++", test_include));
    expect_equal(args_source, args_header);
    expect_equal(args_source, args_include);
  }

  template <> template <>
  void color_coded::config_group::test<7>() /* compilation database, non-existent file defaults. */
  {
    expect_equal
    (
      color_coded::conf::load("compile_commands.json", "c++" "nonexistent/file"),
      color_coded::conf::defaults("c++")
    );
  }

  template <> template <>
  void color_coded::config_group::test<8>() /* Constants still present with compilation database. */
  {
    auto const args(color_coded::conf::load(compile_commands, "c++", test_file));
    for(auto const &constant : color_coded::conf::pre_constants("c++"))
    { expect(std::find(args.begin(), args.end(), constant) != args.end()); }
    for(auto const &constant : color_coded::conf::post_constants())
    { expect(std::find(args.begin(), args.end(), constant) != args.end()); }
  }

  template <> template <>
  void color_coded::config_group::test<9>() /* Pre/post constants in the right positions. */
  {
    auto const args(color_coded::conf::load(test_config, "c++"));
    auto const pre_constants(color_coded::conf::pre_constants("c++"));
    auto const post_constants(color_coded::conf::post_constants());
    // The first arg should be a pre-constant.
    expect(std::find(pre_constants.begin(), pre_constants.end(),*args.begin())
           != pre_constants.end());
    // The last arg should be a post-constant.
    expect(std::find(post_constants.begin(), post_constants.end(),*(args.end() - 1))
           != post_constants.end());
  }
}
