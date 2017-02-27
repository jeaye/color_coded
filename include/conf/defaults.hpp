#pragma once

#include <vector>
#include <string>

#include "env/environment.hpp"

namespace color_coded
{
  namespace conf
  {
    using args_t = std::vector<std::string>;

    /* Prefixed onto every set of args to make life easier. */
    inline args_t pre_constants(std::string const &filetype)
    {
      /* These C++ include paths must always precede /usr/include and alike. */
      args_t cpp_includes =
      {
        /* Local clang+llvm */
        environment<env::tag>::clang_include_cpp,
        environment<env::tag>::clang_include,
#ifdef __APPLE__
        /* System clang on macOS */
        "-isystem/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1",
        "-isystem/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include",
#endif
      };

      if(filetype == "c")
      {
        return
        {
          "-x", "c",
        };
      }
      else if(filetype == "objective-c")
      {
        return
        {
          "-x", "objective-c",
        };
      }
      else if(filetype == "objective-c++")
      {
        args_t args =
        {
          "-x", "objective-c++",
        };
        std::move(cpp_includes.begin(), cpp_includes.end(),
                  std::back_inserter(args));
        return args;
      }
      else // C++ or something else
      {
        args_t args =
        {
          "-x", "c++",
          "-std=c++14",
        };
        std::move(cpp_includes.begin(), cpp_includes.end(),
                  std::back_inserter(args));
        return args;
      }
    }

    inline args_t post_constants()
    {
      return
      {
        "-isystem/usr/local/include",
        "-isystem/opt/local/include",
        environment<env::tag>::clang_resource_dir, // internal libraries and intrinsics
        "-isystem/usr/include",
#ifdef __APPLE__
        "-isystem/System/Library/Frameworks",
        "-isystem/Library/Frameworks",
#endif
        "-w",
        "-fcolor-diagnostics" // See https://github.com/jeaye/color_coded/issues/104
      };
    }

    /* Add system defaults to user specified arguments. Needed because libclang
     * often fails to find system search paths. */
    inline args_t add_defaults_to_args(std::string const &filetype, args_t &&args)
    {
      auto const pre_additions(pre_constants(filetype));
      static auto const post_additions(post_constants());

      args.insert(args.begin(), pre_additions.begin(), pre_additions.end());
      std::copy(std::begin(post_additions), std::end(post_additions),
                std::back_inserter(args));
      return std::move(args);
    }

    /* If no .color_coded file is provided, these are used. */
    inline args_t defaults(std::string const &filetype)
    {
      // Heuristic local includes.
      args_t args{ "-I.", "-Iinclude" };
      return add_defaults_to_args(filetype, std::move(args));
    }
  }
}
