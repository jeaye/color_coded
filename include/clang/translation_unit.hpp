#pragma once

#include <algorithm>

#include "index.hpp"
#include "detail/util.hpp"

namespace color_coded
{
  namespace clang
  {
    static auto const constexpr args
    (
      color_coded::detail::make_array
      (
        "-std=c++1y", "-stdlib=libc++", "-I/usr/include",
        "-I/usr/lib/clang/3.5.0/include", "-I.", "-Iinclude",
        "-Ilib/juble/include", "-Ilib/juble/lib/ruby/include",
        "-Ilib/juble/lib/ruby/.ext/include/x86_64-linux", "-w"
      )
    );

    using translation_unit_impl = CXTranslationUnit;

    struct translation_unit
    {
      translation_unit() = delete;
      translation_unit(index_ptr const ind, std::string const &file)
        : impl
          { clang_parseTranslationUnit(ind->get(), file.c_str(),
            args.data(), args.size(), nullptr, 0, CXTranslationUnit_None) }
        , filename{ file }
        , index{ ind }
      { }
      ~translation_unit()
      { clang_disposeTranslationUnit(impl); }

      translation_unit_impl impl;
      std::string filename;
      index_ptr index;
    };
  }
}
