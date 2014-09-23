#pragma once

#include <string>
#include <fstream>
#include <stdexcept>

namespace color_coded
{
  namespace filesystem
  {
    inline std::size_t file_size(std::string const &file)
    {
      std::ifstream ifs{ file };
      if(!ifs.is_open())
      { throw std::runtime_error{ "failed to open " + file }; }
      ifs.seekg(0, std::ios::end);
      return ifs.tellg();
    }
  }
}
