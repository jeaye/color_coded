#pragma once

#include <string>
#include <stdexcept>

#include <boost/filesystem.hpp>

#include <clang-c/Index.h>

#include "translation_unit.hpp"

namespace color_coded
{
  namespace clang
  {
    using source_range_t = CXSourceRange;

    source_range_t source_range(translation_unit const &trans_unit)
    {
      auto &tu(trans_unit.impl);

      CXFile const file{ clang_getFile(tu, trans_unit.filename.c_str()) };
      std::size_t const size{ boost::filesystem::file_size(trans_unit.filename) };

      CXSourceLocation const top(clang_getLocationForOffset(tu, file, 0));
      CXSourceLocation const bottom(clang_getLocationForOffset(tu, file, size));
      if(clang_equalLocations(top,  clang_getNullLocation()) ||
         clang_equalLocations(bottom, clang_getNullLocation()))
      { throw std::runtime_error{ "cannot retrieve location" }; }

      source_range_t const range(clang_getRange(top, bottom));
      if(clang_Range_isNull(range))
      { throw std::runtime_error{ "cannot retrieve range" }; }

      return range;
    }
  }
}
