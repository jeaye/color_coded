#pragma once

#include <string>
#include <stdexcept>

#include <clang-c/Index.h>

#include "translation_unit.hpp"
#include "detail/filesystem.hpp"

namespace color_coded
{
  namespace clang
  {
    inline CXSourceRange source_range(translation_unit const &trans_unit,
                                      std::string const &filename)
    {
      auto &tu(trans_unit.get());

      CXFile const file{ clang_getFile(tu, filename.c_str()) };
      std::size_t const size{ filesystem::file_size(filename) };

      CXSourceLocation const top(clang_getLocationForOffset(tu, file, 0));
      CXSourceLocation const bottom(clang_getLocationForOffset(tu, file, size));
      if(clang_equalLocations(top,  clang_getNullLocation()) ||
         clang_equalLocations(bottom, clang_getNullLocation()))
      { throw std::runtime_error{ "cannot retrieve location" }; }

      CXSourceRange const range(clang_getRange(top, bottom));
      if(clang_Range_isNull(range))
      { throw std::runtime_error{ "cannot retrieve range" }; }

      return range;
    }
  }
}
