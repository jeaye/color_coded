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

namespace color_coded
{
  void work(std::string const &file, std::string const &data)
  {
    static async::queue<async::task, async::result> q
    {
      [](async::task const &t)
      {
        /* TODO: wrap temp file in RAII to delete when done. */
        std::string const filename{ t.file + ".color_coded.cpp" };
        {
          std::ofstream ofs{ filename };
          ofs << t.code << std::endl;
        }
        try
        {
          clang::translation_unit trans_unit{ clang::compile(filename) };
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
{
  script::registrar::add(script::func(&color_coded::work, "cc_work"));
}
