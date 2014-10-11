#pragma once

#include "clang/string.hpp"
#include "clang/token_pack.hpp"
#include "clang/translation_unit.hpp"
#include "clang/index.hpp"
#include "clang/resource.hpp"
#include "clang/token.hpp"
#include "clang/source_range.hpp"
#include "clang/compile.hpp"

#include "ruby/vim.hpp"

#include "async/queue.hpp"
#include "async/task.hpp"
#include "async/temp_file.hpp"

#include "conf/find.hpp"
#include "conf/load.hpp"
#include "conf/defaults.hpp"

/* XXX: It's a shared object to a C lib; I need globals. :| */
namespace color_coded
{
  namespace core
  {
    namespace fs = boost::filesystem;

    static std::string temp_dir()
    {
      static auto dir(fs::temp_directory_path() / "color_coded/");
      static auto make_dir(fs::create_directory(dir));
      static_cast<void>(make_dir);
      return dir.string();
    }

    static auto& queue()
    {
      static async::queue<async::task, async::result> q
      {
        [](async::task const &t)
        {
          static conf::args_t const config_args_impl{ conf::load(conf::find(".")) };
          fs::path const path{ t.file };
          conf::args_t config_args{ config_args_impl };
          config_args.emplace_back("-I" + fs::absolute(path.parent_path()).string());
          std::string const filename{ temp_dir() + path.filename().string() };
          async::temp_file tmp{ filename, t.code };
          try
          {
            clang::translation_unit trans_unit{ clang::compile({ config_args },
                                                               filename) };
            clang::token_pack tp{ trans_unit, clang::source_range(trans_unit) };
            return async::result{ { trans_unit, tp } };
          }
          catch(clang::compilation_error const &e)
          {
            /* TODO: We shouldn't just blindly log every error, but this
             * will be helpful for meow. */
            ruby::vim::message(e.what());
            return async::result{{}};
          }
          catch(...)
          {
            ruby::vim::message("unknown compilation error");
            return async::result{{}};
          }
        }
      };
      return q;
    }
  }
}
