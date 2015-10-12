#pragma once

#include <string>

#include <boost/filesystem.hpp>

namespace color_coded
{
  namespace conf
  {
    namespace fs = boost::filesystem;

    inline std::string find(fs::path curr)
    {
      fs::directory_iterator const end;
      while(true)
      {
        auto const it
        (
          std::find_if
          (
            fs::directory_iterator{ curr }, end,
            [](fs::directory_entry const &e)
            { return e.path().filename().string() == ".color_coded"; }
          )
        );
        if(it != end)
        { return it->path().string(); }
        else if(!fs::canonical(curr).has_parent_path())
        { return {}; }
        curr /= "..";
      }
      return {};
    }
  }
}
