#pragma once

#include <string>
#include <fstream>
#include <stdexcept>

#include <boost/filesystem.hpp>

namespace color_coded
{
  namespace filesystem
  {
    namespace fs = boost::filesystem;
    using fs::file_size;

    fs::path make_relative(fs::path from, fs::path to)
    {
      from = fs::absolute(from);
      to = fs::absolute(to);
      fs::path::const_iterator from_it{ from.begin() }, to_it{ to.begin() };
      fs::path::const_iterator const to_end{ to.end() }, from_end{ from.end() };

      /* Find common base. */
      while(from_it != from_end && to_it != to_end && *from_it == *to_it)
      { ++from_it, ++to_it; }

      /* Navigate backward to reach the previously found base. */
      fs::path ret;
      std::for_each(from_it, from_end, [&](auto const &i)
      {
        if(i != ".")
        { ret /= ".."; }
      });

      /* Now navigate down the directory branch. */
      std::for_each(to_it, to_end, [&](auto const &i)
      { ret /= i; });

      return ret;
    }
  }
}
