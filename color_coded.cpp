#include "ruby.h"

#include <juble/juble.hpp>

void append(std::string const &str)
{ rb_eval_string(("VIM::Buffer.current.append(0, '" + str + "')").c_str()); }

extern "C" void Init_color_coded()
{
  script::registrar::add(script::func(&append, "cc_append"));
}
