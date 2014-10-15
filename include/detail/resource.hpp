#pragma once

#include <algorithm>
#include <functional>

namespace color_coded
{
  namespace detail
  {
    /* RAII wrapper for non-pointer resources. */
    template <typename T>
    class resource
    {
      public:
        using dtor_t = std::function<void (T&)>;

        resource() = delete;
        resource(dtor_t const &dtor)
          : dtor_{ dtor }
        { }
        resource(resource const&) = delete;
        resource(resource &&) = default;
        resource(T &&t, dtor_t const &dtor)
          : data_(std::move(t)), dtor_{ dtor }
        { }
        ~resource()
        { dtor_(data_); }

        resource& operator =(resource const&) = delete;
        resource& operator =(resource &&r) noexcept
        {
          dtor_(data_);
          data_ = std::move(r.data_);
          return *this;
        }
        resource& operator =(T &&t) noexcept
        {
          dtor_(data_);
          data_ = std::move(t);
          return *this;
        }

        T& get()
        { return data_; }
        T const& get() const
        { return data_; }

      private:
        T data_{};
        dtor_t dtor_;
    };
  }
}
