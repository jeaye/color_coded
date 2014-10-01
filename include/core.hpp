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

/* TODO: This is temporary; come up with something better. */
namespace color_coded
{
  struct core
  {
    static core& get()
    {
      static core c;
      return c;
    }

    static conf::args_t config_args;
    static async::queue<async::task, async::result> q;
  };
  conf::args_t core::config_args{ conf::load(conf::find(".")) };
  async::queue<async::task, async::result> core::q
  {
    [](async::task const &t)
    {
      std::string const filename{ t.file + ".color_coded.cpp" };
      async::temp_file tmp{ filename, t.code };
      try
      {
        clang::translation_unit trans_unit{ clang::compile({ config_args },
                                                           filename) };
        clang::token_pack tp{ trans_unit, clang::source_range(trans_unit) };
        return async::result{ { trans_unit, tp } };
      }
      catch(clang::compilation_error const&)
      { return async::result{{}}; }
    }
  };
}
