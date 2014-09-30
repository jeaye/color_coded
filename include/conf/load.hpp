#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>

#include "defaults.hpp"

namespace color_coded
{
  namespace conf
  {
    inline args_t load(std::string const &file)
    {
      if(file.empty())
      { return defaults(); }

      std::ifstream ifs{ file };
      if(!ifs.is_open())
      { return defaults(); }

      args_t args;
      std::string tmp;
      while(std::getline(ifs, tmp))
      { args.emplace_back(std::move(tmp)); }

      return args;
    }
  }
}
