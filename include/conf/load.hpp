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
      inline std::string make_absolute(std::string line)
      {
        static std::regex const reg
        {
          "\\s*(-I|-isystem|-iquote|--sysroot=)\\s*(.*)",
          std::regex::optimize
        };
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

    inline args_t load(std::string const &file, std::string const &filetype)
    {
      if(file.empty())
      { return defaults(filetype); }

      std::ifstream ifs{ file };
      if(!ifs.is_open())
      { return defaults(filetype); }

      auto const pre_additions(pre_constants(filetype));
      static auto const post_additions(post_constants());
      args_t args{ pre_additions };

      std::string tmp;
      while(std::getline(ifs, tmp))
      { args.emplace_back(detail::make_absolute(std::move(tmp))); }

      std::copy(post_additions.begin(), post_additions.end(),
                std::back_inserter(args));

      return args;
    }
  }
}
