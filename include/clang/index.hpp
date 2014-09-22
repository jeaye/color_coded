#pragma once

#include <algorithm>

#include "resource.hpp"

namespace color_coded
{
  namespace clang
  {
    namespace detail
    {
      template <>
      struct resource_impl<CXIndex>
      {
        static void deleter(CXIndex &index)
        { clang_disposeIndex(index); }
      };
    }

    using index = resource<CXIndex>;
  }
}
