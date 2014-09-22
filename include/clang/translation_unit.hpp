#pragma once

#include <algorithm>
#include <clang-c/CXString.h>

#include "resource.hpp"

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

    using translation_unit = resource<CXTranslationUnit>;
  }
}
