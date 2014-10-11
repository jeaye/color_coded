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
  static void pull(std::string const &file)
  {
    auto const pulled(core::queue().pull());
    if(pulled.second)
    {
      auto &buf(core::buffers()[pulled.first.name]);
      buf.group = std::move(pulled.first.group);
      if(file == pulled.first.name)
      { vim::apply(buf.group); }
    }
  }

  static void push(std::string const &file, std::string const &data)
  {
    pull(file);
    core::queue().push({ file, data });
  }

  static void enter(std::string const &file, std::string const &data)
  {
    auto &buf(core::buffers()[file]);
    if(buf.group.size())
    { vim::apply(buf.group); }
    else
    { ruby::vim::clearmatches(); }
    core::queue().push({ file, data });
  }

  static void destroy(std::string const &file)
  { core::buffers().erase(file); }
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
  script::registrar::add(script::func
    (color_coded::safe_func<decltype(&color_coded::destroy), &color_coded::destroy>(),
    "color_coded_destroy"));
}
