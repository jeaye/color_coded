#pragma once

#include <string>

#include <boost/filesystem.hpp>

namespace color_coded
{
  namespace conf
  {
    namespace fs = boost::filesystem;

    inline fs::directory_iterator find_file(fs::path const &dir, std::string const &file)
    {
      static fs::directory_iterator const end;
      return std::find_if
      (
          fs::directory_iterator{ dir }, end,
          [&](fs::directory_entry const &e)
          { return e.path().filename().string() == file; }
      );
    }

    /* From the current directory, search for a .color_coded file for the
     * current filetype first, then for a non-typed .color_coded. If neither
     * is found, move to the parent directory. Repeat until /. */
    inline std::string find(fs::path file, std::string const &filetype)
    {
      fs::path curr{ file.parent_path() };
      static fs::directory_iterator const end;

      static auto constexpr file_name(".color_coded");
      static auto constexpr compilation_database("compile_commands.json");
      auto const typed_file_name(std::string{ file_name } + "_" + filetype);
      std::string const flag_files[] {typed_file_name, compilation_database, file_name};

      while(true)
      {

        for(auto const &file : flag_files)
        {
          auto const dir_it(find_file(curr, file));
          if(dir_it != end)
          { return dir_it->path().string(); }
        }

        /* We may have hit /. */
        if(!fs::canonical(curr).has_parent_path())
        { return {}; }

        curr /= "..";
      }
      return {};
    }
  }
}
