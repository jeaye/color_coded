#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <iostream>

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

namespace color_coded
{
  void work(std::string const &file, std::string const &data)
  {
    static conf::args_t config_args{ conf::load(conf::find(".")) };
    static async::queue<async::task, async::result> q
    {
      [&](async::task const &t)
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

    auto const pulled(q.pull());
    if(pulled.second)
    { vim::apply(pulled.first.group); }

    q.push({ file, data });
  }
}

extern "C" void Init_color_coded()
{ script::registrar::add(script::func(&color_coded::work, "cc_work")); }
