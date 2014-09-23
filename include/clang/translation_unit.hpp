#pragma once

#include <algorithm>

#include "index.hpp"
#include "resource.hpp"
#include "detail/util.hpp"

namespace color_coded
{
  namespace clang
  {
    namespace detail
    {
      template <>
      struct resource_impl<CXTranslationUnit>
      {
        static void deleter(CXTranslationUnit &tu)
        { clang_disposeTranslationUnit(tu); }
      };
    }
    using translation_unit_impl = resource<CXTranslationUnit>;

    static auto const constexpr args(
    color_coded::detail::make_array
    (
      "-std=c++1y", "-stdlib=libc++", "-I/usr/include",
      "-I/usr/lib/clang/3.5.0/include", "-I.", "-Iinclude",
      "-Ilib/juble/include", "-Ilib/juble/lib/ruby/include",
      "-Ilib/juble/lib/ruby/.ext/include/x86_64-linux"
    ));

    struct translation_unit
    {
      translation_unit() = delete;
      translation_unit(clang::index const &index, std::string const &file)
        : impl
          { clang_parseTranslationUnit(index.get(), file.c_str(),
            args.data(), args.size(), nullptr, 0, CXTranslationUnit_None) }
        , filename{ file }
      { }

      translation_unit_impl const impl;
      std::string const filename;
    };
  }
}
