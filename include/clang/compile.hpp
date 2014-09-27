#pragma once

#include <string>

#include <clang-c/Index.h>

#include "index.hpp"
#include "translation_unit.hpp"
#include "string.hpp"
#include "token_pack.hpp"

namespace color_coded
{
  namespace clang
  {
    struct compilation_error : std::runtime_error
    { using std::runtime_error::runtime_error; };

    inline translation_unit compile(std::string const &filename)
    {
      auto const index(std::make_shared<index>(clang_createIndex(false, false)));
      translation_unit const trans_unit{ index, filename };
      auto &tu(trans_unit.impl);

      std::size_t const errors{ clang_getNumDiagnostics(tu) };
      if(errors || !tu)
      {
        for(std::size_t i{}; i != errors; ++i)
        {
          CXDiagnostic const diag{ clang_getDiagnostic(tu, i) };
          string const str{ clang_formatDiagnostic(diag,
                            clang_defaultDiagnosticDisplayOptions()) };
        }
        throw compilation_error{ "unable to compile translation unit" };
      }

      return trans_unit;
    }
  }
}
