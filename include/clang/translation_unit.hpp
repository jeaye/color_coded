#pragma once

#include <algorithm>

#include "index.hpp"
#include "detail/util.hpp"
#include "conf/args_view.hpp"

namespace color_coded
{
  namespace clang
  {
    using translation_unit_impl = CXTranslationUnit;

    struct translation_unit
    {
      translation_unit() = delete;
      translation_unit(conf::args_view const &args, index_ptr const ind,
                       std::string const &file)
        : impl
          { clang_parseTranslationUnit(ind->get(), file.c_str(),
            args.data(), args.size(), nullptr, 0,
            CXTranslationUnit_DetailedPreprocessingRecord) }
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
