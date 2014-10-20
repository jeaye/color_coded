#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <iostream>

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include "detail/safe_func.hpp"

namespace color_coded
{
  static bool pull_impl(std::string const &file)
  {
    auto const pulled(core::queue().pull());
    if(pulled.second)
    {
      auto &buf(core::buffers()[pulled.first.name]);
      buf.group = std::move(pulled.first.group);
      return file == pulled.first.name;
    }
    return false;
  }

  static int pull(lua_State * const lua)
  {
    auto const file(lua_tostring(lua, -1));
    lua_pushboolean(lua, pull_impl(file));
    return 1;
  }

  static void push_impl(std::string const &file, std::string const &data)
  {
    if(pull_impl(file))
    { vim::apply(core::buffers()[file]); }
    core::queue().push({ file, data });
  }

  static int push(lua_State * const lua)
  {
    auto const file(lua_tostring(lua, -2));
    auto const data(lua_tostring(lua, -1));
    push_impl(file, data);
    return 0;
  }

  static void moved_impl(std::string const &file, std::size_t const line,
                         std::size_t const lines, std::size_t const height)
  {
    auto &buf(core::buffers()[file]);
    if(pull_impl(file))
    { vim::apply(buf); }
    else
    {
      buf.line = line;
      buf.lines = lines;
      buf.height = height;
      vim::try_apply(buf);
    }
  }

  static int moved(lua_State * const lua)
  {
    auto const file(lua_tostring(lua, -4));
    auto const line(lua_tonumber(lua, -3));
    auto const lines(lua_tonumber(lua, -2));
    auto const height(lua_tonumber(lua, -1));
    moved_impl(file, line, lines, height);
    return 0;
  }

  static void enter_impl(std::string const &file, std::string const &data)
  {
    auto &buf(core::buffers()[file]);
    if(buf.group.size())
    { vim::apply(buf); }
    else
    { ruby::vim::clearmatches(); }
    core::queue().push({ file, data });
  }

  static int enter(lua_State * const lua)
  {
    auto const data(lua_tostring(lua, -1));
    auto const file(lua_tostring(lua, -2));
    enter_impl(file, data);
    return 0;
  }

  static void destroy_impl(std::string const &file)
  { core::buffers().erase(file); }

  static int destroy(lua_State * const lua)
  {
    auto const file(lua_tostring(lua, -1));
    destroy_impl(file);
    return 0;
  }

  static std::string last_error_impl()
  { return core::last_error(); }

  static int last_error(lua_State * const lua)
  {
    lua_pushstring(lua, last_error_impl().c_str());
    return 1;
  }
}

extern "C" int luaopen_color_coded(lua_State * const lua)
{
  color_coded::ruby::lua_state(lua);
  lua_register(lua, "color_coded_pull", &color_coded::pull);
  lua_register(lua, "color_coded_push", &color_coded::push);
  lua_register(lua, "color_coded_moved", &color_coded::moved);
  lua_register(lua, "color_coded_enter", &color_coded::enter);
  lua_register(lua, "color_coded_destroy", &color_coded::destroy);
  lua_register(lua, "color_coded_last_error", &color_coded::last_error);
  return 0;
}
