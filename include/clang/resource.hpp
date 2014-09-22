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

    template <typename T>
    class resource : public detail::resource_impl<T>
    {
      public:
        resource() = default;
        resource(resource const&) = default;
        resource(resource &&) = default; /* TODO: noexcept */
        resource(T &&data)
          : data_{ std::move(data), &detail::resource_impl<T>::deleter }
        { }

        resource& operator =(resource const&) = default;
        resource& operator =(resource &&) noexcept = default;
        resource& operator =(T &&data) noexcept
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
