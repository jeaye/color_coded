#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <regex>

#include <boost/filesystem.hpp>

#include "defaults.hpp"
#include <clang/Tooling/JSONCompilationDatabase.h>

namespace color_coded
{
  namespace core
  {
    std::string const& last_error(std::string const &e);
  }

  namespace conf
  {
    namespace fs = boost::filesystem;

    namespace detail
    {
      inline std::string make_absolute(std::string line, fs::path const &base)
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
          { line = match[1].str() + fs::absolute(str, base).string(); }
        }

        return line;
      }

      template <typename It>
      It make_absolute(It const begin, It const end, fs::path const &base)
      {
        static const std::regex reg_dual
        {
          "\\s*(-I|-isystem|-iquote)\\s*$",
            std::regex::optimize
        };

        std::smatch match;
        if (std::regex_search(*begin, match, reg_dual))
        {
          for (auto next = begin + 1; next < end; ++next)
          {
            if (next->size() == 0)
            { continue; }
            else
            {
              auto path = next->c_str();
              if (path[0] != '/')
                *next = fs::absolute(path, base).string();
              return next +1;
            }
          }

          return end + 1;
        }
        else
        {
          *begin = make_absolute(*begin, base);
          return begin +1;
        }
      }
    }

    inline args_t load_compilation_database(std::string const &file, fs::path filename)
    {
      static const std::string source_extensions[] {".c", ".cpp", ".cc"};
      static const std::string header_extensions[] {".h", ".hpp", ".hh"};
      std::string error;
#if LLVM_VERSION_MAJOR >= 4
      auto const database_ptr
      (
        ::clang::tooling::JSONCompilationDatabase::loadFromFile
        (
          file,
          error,
          ::clang::tooling::JSONCommandLineSyntax::Gnu
        )
      );
#else
      auto const database_ptr
      (
        ::clang::tooling::JSONCompilationDatabase::loadFromFile
        (
          file,
          error
        )
      );
#endif
      if(!database_ptr)
      {
          core::last_error(error);
          return {};
      }

      std::vector<fs::path> files{filename};
      auto const ext(filename.extension());
      if(std::find(begin(header_extensions), end(header_extensions), ext) != end(header_extensions))
      {
        auto path = filename.string();
        auto const include_it = path.rfind("include");
        if(include_it != std::string::npos)
        {
            filename = path.replace(include_it, 7, "src");
        }
        for(auto const &extension : source_extensions)
        {
          files.emplace_back(filename.replace_extension(extension));
        }
      }

      std::vector<::clang::tooling::CompileCommand> compile_commands;
      for(auto const &file : files)
      {
        compile_commands = database_ptr->getCompileCommands(file.string());

        if(!compile_commands.empty())
        {
          filename = file;
          break;
        }

      }

      if(compile_commands.empty())
      { return {}; }

      const auto base(fs::path{compile_commands[0].Directory});
      // Skip argv[0] which is the name of the clang executable.
      args_t commands((compile_commands[0].CommandLine.begin() + 1), compile_commands[0].CommandLine.end());

      // Get rid of the source filename itself.
      // NOTE: '-o <output>' and '-c' will be automatically ignored by libclang.
      commands.erase(std::remove(commands.begin(), commands.end(), filename), commands.end());
      commands.erase(std::remove(commands.begin(), commands.end(), compile_commands[0].Filename), commands.end());

      for (auto it = commands.begin(); it < commands.end();)
      { it = detail::make_absolute(it, commands.end(), base); }

      return commands;
    }

    inline args_t load_color_coded(std::string const &file)
    {
      std::ifstream ifs{ file };
      if(!ifs.is_open())
      { return {}; }

      args_t args;

      auto const &base(fs::path{ file }.parent_path());
      std::string tmp;
      while(std::getline(ifs, tmp))
      { args.emplace_back(detail::make_absolute(std::move(tmp), base)); }

      return args;
    }

    inline args_t load(std::string const &file, std::string const &filetype, std::string const &filename)
    {
      if(file.empty())
      { return defaults(filetype); }

      args_t args;
      if (fs::path(file).filename() == "compile_commands.json")
      { args = load_compilation_database(file, filename); }
      else
      { args = load_color_coded(file); }

      if(args.empty())
      { return defaults(filetype); }

      return add_defaults_to_args(filetype, std::move(args));
    }

    inline args_t load(std::string const &file, std::string const &filetype)
    { return load(file, filetype, "");}
  }
}
