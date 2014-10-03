#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <iostream>

#include "core.hpp"

namespace color_coded
{
  static void pull()
  {
    auto const pulled(core::queue().pull());
    if(pulled.second)
    { vim::apply(pulled.first.group); }
  }

  static void push(std::string const &file, std::string const &data)
  {
    pull();
    core::queue().push({ file, data });
  }
}

extern "C" void Init_color_coded()
{
  script::registrar::add(script::func(&color_coded::pull, "color_coded_pull"));
  script::registrar::add(script::func(&color_coded::push, "color_coded_push"));
}
