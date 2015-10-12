#pragma once

#include <string>

#include <boost/filesystem.hpp>

namespace color_coded
{
  namespace conf
  {
    namespace fs = boost::filesystem;

    /* From the current directory, search for a .color_coded file for the
     * current filetype first, then for a non-typed .color_coded. If neither
     * is found, move to the parent directory. Repeat until /. */
    inline std::string find(fs::path curr, std::string const &filetype)
    {
      fs::directory_iterator const end;
      static auto constexpr file_name(".color_coded");
      auto const typed_file_name(std::string{ file_name } + "_" + filetype);
      while(true)
      {
        auto const typed_it
        (
          std::find_if
          (
            fs::directory_iterator{ curr }, end,
            [&](fs::directory_entry const &e)
            { return e.path().filename().string() == typed_file_name; }
          )
        );
        if(typed_it != end)
        { return typed_it->path().string(); }

        auto const normal_it
        (
          std::find_if
          (
            fs::directory_iterator{ curr }, end,
            [&](fs::directory_entry const &e)
            { return e.path().filename().string() == file_name; }
          )
        );
        if(normal_it != end)
        { return normal_it->path().string(); }

        /* We may have hit /. */
        if(!fs::canonical(curr).has_parent_path())
        { return {}; }

        curr /= "..";
      }
      return {};
    }
  }
}
