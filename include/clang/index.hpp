#pragma once

#include <memory>

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
    using index_ptr = std::shared_ptr<index const>;
  }
}
