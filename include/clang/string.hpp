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
      struct resource_impl<CXString>
      {
        char const * c_str() const
        {
          auto const &self(static_cast<resource<CXString> const&>(*this));
          return clang_getCString(self.get());
        }

        static void deleter(CXString &str)
        { clang_disposeString(str); }
      };
    }

    using string = resource<CXString>;
  }
}
