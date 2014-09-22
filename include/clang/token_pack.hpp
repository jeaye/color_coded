#pragma once

#include <clang-c/Index.h>

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
        token_pack(CXTranslationUnit const &tu, CXSourceRange const &range)
          : tu_{ tu }
          , range_{ range }
        {
          unsigned num{};
          clang_tokenize(tu_, range_, &data_, &num);
          size_ = num;
        }
        ~token_pack()
        { clang_disposeTokens(tu_, data_, size_); }
          
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
        CXToken *data_{};
        std::size_t size_{};
        CXTranslationUnit const &tu_;
        CXSourceRange const &range_;
    };
  }
}
