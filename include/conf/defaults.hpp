#pragma once

#include <vector>
#include <string>

namespace color_coded
{
  namespace conf
  {
    using args_t = std::vector<std::string>;

    inline args_t constants()
    {
      return
      { 
        "-stdlib=libc++",
        "-I/usr/include",
        "-I/usr/lib/clang/3.5.0/include",
        "-I/usr/lib/clang/3.4/include",
        "-w"
      };
    }

    inline args_t defaults()
    {
      static auto const additions(constants());
      args_t args{ "-std=c++1y", "-I.", "-Iinclude" };
      std::copy(std::begin(additions), std::end(additions),
                std::back_inserter(args));
      return args;
    }
  }
}
