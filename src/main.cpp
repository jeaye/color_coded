#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <iostream>

#include "detail/safe_func.hpp"

namespace color_coded
{
  static bool pull(std::string const &file)
  {
    auto const pulled(core::queue().pull());
    if(pulled.second)
    {
      /* Check if any compilations have finished, resulting in
       * new highlighting groups. */
      auto &buf(core::buffers()[pulled.first.name]);
      buf.group = std::move(pulled.first.group);
      return file == pulled.first.name;
    }
    return false;
  }

  static void push(std::string const &file, std::string const &data)
  { core::queue().push({ file, data }); }

  static void moved(std::string const &file, std::size_t const line,
                    std::size_t const lines, std::size_t const height)
  {
    auto &buf(core::buffers()[file]);
    buf.line = line;
    buf.lines = lines;
    buf.height = height;

    /* Moving may result in re-highlighting given two cases:
     *  1. New highlighting information was pulled (a compilation finished)
     *  2. We approach the edge of our highlighting range
     */
    if(pull(file))
    { vim::apply(buf); }
    else
    { vim::try_apply(buf); }
  }

  static void enter(std::string const &file, std::string const &data)
  {
    /* When entering a new buffer, check if we have highlight data for
     * it already. If not, clear any highlighting in the file. */
    auto &buf(core::buffers()[file]);
    if(buf.group.size())
    { vim::apply(buf); }
    else
    { ruby::vim::clearmatches(); }
    core::queue().push({ file, data });
  }

  static void destroy(std::string const &file)
  { core::buffers().erase(file); }

  static std::string last_error()
  { return core::last_error(); }
}

extern "C" void Init_color_coded()
{
  script::registrar::add(script::func
    (color_coded::safe_func<decltype(&color_coded::pull),
                            &color_coded::pull>(),
    "color_coded_pull"));
  script::registrar::add(script::func
    (color_coded::safe_func<decltype(&color_coded::push),
                            &color_coded::push>(),
    "color_coded_push"));
  script::registrar::add(script::func
    (color_coded::safe_func<decltype(&color_coded::moved),
                            &color_coded::moved>(),
    "color_coded_moved"));
  script::registrar::add(script::func
    (color_coded::safe_func<decltype(&color_coded::enter),
                            &color_coded::enter>(),
    "color_coded_enter"));
  script::registrar::add(script::func
    (color_coded::safe_func<decltype(&color_coded::destroy),
                            &color_coded::destroy>(),
    "color_coded_destroy"));
  script::registrar::add(script::func
    (color_coded::safe_func<decltype(&color_coded::last_error),
                            &color_coded::last_error>(),
    "color_coded_last_error"));
}
