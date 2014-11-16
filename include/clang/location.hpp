#pragma once

#include <clang-c/Index.h>

#include "string.hpp"

namespace color_coded
{
  namespace clang
  {
    struct location
    {
      clang::string filename{};
      CXFile file{};
      unsigned start_line{}, start_col{}, start_offset{};
      unsigned end_line{}, end_col{}, end_offset{};
    };

    std::ostream& operator <<(std::ostream &os, location const &loc)
    {
      //{
      //  auto const c_str(loc.filename.c_str());
      //  if(c_str)
      //  { os << c_str << " "; }
      //}
      //{
      //  clang::string const str(clang_getFileName(loc.file));
      //  auto const f_str(str.c_str());
      //  if(f_str)
      //  { os << f_str << " "; }
      //}
      return os << loc.start_line << ":" << loc.start_col << " -> "
                << loc.end_line << ":" << loc.end_col;
    }

    inline location make_spelling_location(CXSourceLocation const start,
                                           CXSourceLocation const end)
    {
      location ret;
      clang_getSpellingLocation(start, &ret.file,
                                &ret.start_line, &ret.start_col,
                                &ret.start_offset);
      clang_getSpellingLocation(end, &ret.file,
                                &ret.end_line, &ret.end_col,
                                &ret.end_offset);
      return ret;
    }

    inline location make_presumed_location(CXSourceLocation const start,
                                           CXSourceLocation const end)
    {
      location ret;
      clang_getPresumedLocation(start, &ret.filename.get(),
                                &ret.start_line, &ret.start_col);
      clang_getPresumedLocation(end, &ret.filename.get(),
                                &ret.end_line, &ret.end_col);
      return ret;
    }

    inline location make_expansion_location(CXSourceLocation const start,
                                            CXSourceLocation const end)
    {
      location ret;
      clang_getExpansionLocation(start, &ret.file, &ret.start_line,
                                 &ret.start_col, &ret.start_offset);
      clang_getExpansionLocation(end, &ret.file, &ret.end_line,
                                 &ret.end_col, &ret.end_offset);
      return ret;
    }
  }
}
