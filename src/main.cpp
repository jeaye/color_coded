#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <iostream>

#include "core.hpp"
#include "detail/safe_func.hpp"

namespace color_coded
{
  void pull()
  {
    auto const pulled(core::queue().pull());
    if(pulled.second)
    { vim::apply(pulled.first.group); }
  }

  void push(std::string const &file, std::string const &data)
  {
    pull();
    core::queue().push({ file, data });
  }

  void enter(std::string const &file, std::string const &data)
  {
    ruby::vim::clearmatches();
    core::queue().push({ file, data });
  }
}

extern "C" void Init_color_coded()
{
  script::registrar::add(script::func
    (color_coded::safe_func<decltype(&color_coded::pull), &color_coded::pull>(),
    "color_coded_pull"));
  script::registrar::add(script::func
    (color_coded::safe_func<decltype(&color_coded::push), &color_coded::push>(),
    "color_coded_push"));
  script::registrar::add(script::func
    (color_coded::safe_func<decltype(&color_coded::enter), &color_coded::enter>(),
    "color_coded_enter"));
}
