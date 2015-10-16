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
        return
        {
          "-x", "objective-c",
        };
      }
      else // C++ or something else
      {
        return
        {
          "-x", "c++",
          "-std=c++14",
        };
      }
    }

    inline args_t post_constants()
    {
      return
      {
        environment<env::tag>::clang_include,
        environment<env::tag>::clang_include_cpp,
        environment<env::tag>::clang_lib_include,
        "-I/usr/local/include",
        "-I/opt/local/include",
        "-I/usr/include",
        "-w"
      };
    }

    /* If no .color_coded file is provided, these are used. */
    inline args_t defaults(std::string const &filetype)
    {
      auto const pre_additions(pre_constants(filetype));
      static auto const post_additions(post_constants());
      args_t args{ "-I.", "-Iinclude" };
      std::copy(std::begin(pre_additions), std::end(pre_additions),
                std::back_inserter(args));
      std::copy(std::begin(post_additions), std::end(post_additions),
                std::back_inserter(args));
      return args;
    }
  }
}
