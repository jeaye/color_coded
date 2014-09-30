#pragma once

#include <algorithm>

#include "load.hpp"

namespace color_coded
{
  namespace conf
  {
    class args_view
    {
      public:
        using view_t = std::vector<char const *>;
        using iterator = view_t::iterator;
        using const_iterator = view_t::const_iterator;
        using size_type = std::size_t;

        args_view() = delete;
        args_view(args_t const &args)
        {
          std::transform(std::begin(args), std::end(args),
                         std::back_inserter(view_), [](auto const &s)
                         { return s.c_str(); });
        }

        const_iterator begin() const
        { return view_.begin(); }
        const_iterator end() const
        { return view_.end(); }

        char const * const * data() const
        { return view_.data(); }

        size_type size() const
        { return view_.size(); }

      private:
        view_t view_;
    };
  }
}
