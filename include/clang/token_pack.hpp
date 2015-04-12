#pragma once

#include <clang-c/Index.h>

#include "translation_unit.hpp"
#include "source_range.hpp"

namespace color_coded
{
  namespace clang
  {
    class token_pack
    {
      public:
        using token_t = CXToken;
        using iterator = token_t*;
        using const_iterator = token_t const *;

        token_pack() = delete;
        token_pack(translation_unit const &tu, source_range_t const &range)
          : tu_{ tu }
          , range_{ range }
        {
          unsigned num{};
          clang_tokenize(tu_.impl, range_, &data_, &num);
          size_ = num;
        }
        ~token_pack()
        { clang_disposeTokens(tu_.impl, data_, size_); }

        iterator begin()
        { return data_; }
        const_iterator cbegin() const
        { return data_; }
        iterator end()
        { return data_ + size_; }
        const_iterator cend() const
        { return data_ + size_; }

        std::size_t size() const
        { return size_; }

      private:
        token_t *data_{};
        std::size_t size_{};
        translation_unit const &tu_;
        source_range_t const &range_;
    };
  }
}
