#include "ruby.h"

#include <juble/juble.hpp>

namespace color_coded
{
  void eval(std::string const &str)
  { rb_eval_string(str.c_str()); }

  void append(std::string const &str)
  { eval("VIM::Buffer.current.append(0, '" + str + "')"); }

  void work(std::string const &line)
  {
    std::string const len{ std::to_string(line.size()) };
    eval("VIM::command(\"call matchaddpos('keyword', [0, 0, " + len  + "], -1)\")");
  }
}

extern "C" void Init_color_coded()
{
  script::registrar::add(script::func(&color_coded::append, "cc_append"));
  script::registrar::add(script::func(&color_coded::work, "cc_work"));
}
