#pragma once

#include "clang/string.hpp"
#include "clang/token_pack.hpp"
#include "clang/translation_unit.hpp"
#include "clang/index.hpp"
#include "clang/resource.hpp"
#include "clang/token.hpp"
#include "clang/source_range.hpp"
#include "clang/compile.hpp"

#include "vim/commands.hpp"
#include "vim/buffer.hpp"

#include "async/queue.hpp"
#include "async/task.hpp"
#include "async/temp_file.hpp"

#include "conf/find.hpp"
#include "conf/load.hpp"
#include "conf/defaults.hpp"

#include <string>
#include <map>

/* XXX: It's a shared object to a C lib; I need globals. :| */
namespace color_coded
{
  namespace core
  {
    namespace fs = boost::filesystem;

    inline std::string temp_dir()
    {
      static auto dir(fs::temp_directory_path() / "color_coded/");
      static auto make_dir(fs::create_directory(dir));
      static_cast<void>(make_dir);
      return dir.string();
    }

    inline std::string const& last_error(std::string const &e = "")
    {
      static std::string error{ "no errors" };
      if(e.size())
      { error = e; }
      return error;
    }

    inline auto& queue()
    {
      static async::queue<async::task, async::result> q
      {
        [](async::task const &t)
        {
          try
          {
            /* Build the compiler arguments. */
            static conf::args_t const config_args_impl
            { conf::load(conf::find(".")) };

            fs::path const path{ t.name };
            conf::args_t config_args{ config_args_impl };
            config_args.emplace_back("-I" +
                                     fs::absolute(path.parent_path()).string());

            std::string const filename{ temp_dir() + path.filename().string() };
            async::temp_file tmp{ filename, t.code };

            /* Attempt compilation. */
            clang::translation_unit trans_unit
            { clang::compile({ config_args }, filename) };
            clang::token_pack tp{ trans_unit, clang::source_range(trans_unit) };

            return async::result{ t.name, { trans_unit, tp } };
          }
          catch(clang::compilation_error const &e)
          { return async::result{{}, {}}; }
          catch(...)
          {
            last_error("unknown compilation error");
            return async::result{{}, {}};
          }
        }
      };

      return q;
    }

    inline auto& buffers()
    {
      static std::map<std::string, vim::buffer> buffers;
      return buffers;
    }
  }
}
