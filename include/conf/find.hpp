#pragma once

#include <string>

#include <boost/filesystem.hpp>

namespace color_coded
{
  namespace conf
  {
    namespace filesystem = boost::filesystem;

    inline std::string find(filesystem::path curr)
    {
      /* TODO: Configurable max depth. */
      filesystem::directory_iterator const end;
      while(true)
      {
        auto const it
        (
          std::find_if
          (
           filesystem::directory_iterator{ curr }, end,
           [](filesystem::directory_entry const &e)
           { return e.path().filename().string() == ".color_coded"; }
          )
        );
        if(it != end)
        { return it->path().string(); }
        else if(!filesystem::canonical(curr).has_parent_path())
        { return {}; }
        curr /= "..";
      }
      return {};
    }
  }
}
