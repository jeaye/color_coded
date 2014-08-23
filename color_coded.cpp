#include "ruby.h"

extern "C" void Init_color_coded()
{
  rb_eval_string("VIM::Buffer.current.append(0, 'initialized from C')");
}
