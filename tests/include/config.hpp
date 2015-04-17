#pragma once

#include <jest/jest.hpp>
#include "conf/load.hpp"
#include "conf/defaults.hpp"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define TESTCONFIG TOSTRING(TEST_ROOT) "/params_test"

struct config_test{  };
using config_group = jest::group<config_test>;
static config_group const config_obj{ "loading and reading of config files" };

namespace jest
{
    template <> template <>
    void config_group::test<1>()
    {
        expect(color_coded::conf::load("") == color_coded::conf::defaults());
    }

    template <> template <>
    void config_group::test<2>()
    {
        expect(color_coded::conf::load("nonexistent/file") == color_coded::conf::defaults());
    }

    template <> template <>
    void config_group::test<3>()
    {
        auto args = color_coded::conf::load(TESTCONFIG);
        expect(std::find(args.begin(), args.end(), "-I" TOSTRING(TEST_ROOT)) != args.end());
        expect(std::find(args.begin(), args.end(), "-I/etc/") != args.end());
        expect(std::find(args.begin(), args.end(), "foo") != args.end());
        expect(args.size() >= 3 + color_coded::conf::constants().size());
    }

    template <> template <>
    void config_group::test<4>()
    {
        auto args = color_coded::conf::load(TESTCONFIG);
        for(const auto &constant : color_coded::conf::constants())
            expect(std::find(args.begin(), args.end(), constant) != args.end());
    }
}
