#pragma once

#include <algorithm>
#include <clang-c/Index.h>

#include "detail/resource.hpp"

namespace color_coded
{
  namespace clang
  {
    namespace detail
    {
      template <typename T>
      struct resource_impl
      {
        static void deleter(T &)
        { }
      };
    }

    /* Extends detail::resource by providing the ability to specialize
     * the resource_impl and provide custom deleters; this is in lieu
     * of embedding them in the type during each instantiation. */
    template <typename T>
    class resource : public detail::resource_impl<T>
    {
      public:
        resource() = default;
        resource(resource const&) = default;
        resource(resource &&) = default;
        resource(T &&data)
          : data_{ std::move(data), &detail::resource_impl<T>::deleter }
        { }

        resource& operator =(resource const&) = default;
        resource& operator =(resource &&) = default;
        resource& operator =(T &&data)
        {
          data_ = std::move(data);
          return *this;
        }

        T& get()
        { return data_.get(); }
        T const& get() const
        { return data_.get(); }

      private:
        color_coded::detail::resource<T> data_{ &detail::resource_impl<T>::deleter };
    };
  }
}
