#pragma once

#include <string>
#include <fstream>

#include <boost/filesystem.hpp>

namespace color_coded
{
  namespace async
  {
    struct temp_file
    {
      temp_file() = delete;
      temp_file(std::string const &filename, std::string const &data)
        : name{ filename }
      { std::ofstream{ name } << data << std::endl; }
      ~temp_file()
      { boost::filesystem::remove(name); }

      std::string const name;
    };
  }
}
