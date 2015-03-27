#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <regex>

#include <boost/filesystem.hpp>

#include "defaults.hpp"

namespace color_coded
{
  namespace conf
  {
    namespace detail
    {
      std::string make_absolute(std::string line)
      {
        static std::regex reg{ "\\s*(-I|-isystem|-iquote|--sysroot=)\\s*(.*)" };
        static std::smatch match;
        if(std::regex_search(line, match, reg))
        {
          auto const &str(match[2].str());
          if(str.size() && str[0] != '/')
          { line = match[1].str() + boost::filesystem::absolute(str).string(); }
        }
        
        return line;
      }
    }

    args_t load(std::string const &file)
    {
      if(file.empty())
      { return defaults(); }

      std::ifstream ifs{ file };
      if(!ifs.is_open())
      { return defaults(); }

      static auto const additions(constants());
      args_t args{ additions };

      std::string tmp;
      while(std::getline(ifs, tmp))
      { args.emplace_back(detail::make_absolute(std::move(tmp))); }

      return args;
    }
  }
}
