extern "C"
{
  #include <lua.h>
  #include <lauxlib.h>
  #include <lualib.h>
}

#include "events.hpp"
#include "detail/safe_func.hpp"

/* Small API exposed to Lua. */
namespace color_coded
{
  int pull(lua_State * const lua)
  {
    auto const file(lua_tostring(lua, -1));
    lua_pushboolean(lua, event::pull(file));
    return 1;
  }

  int push(lua_State * const lua)
  {
    auto const file(lua_tostring(lua, -2));
    auto const data(lua_tostring(lua, -1));
    event::push(file, data);
    return 0;
  }

  int moved(lua_State * const lua)
  {
    auto const file(lua_tostring(lua, -4));
    auto const line(lua_tonumber(lua, -3));
    auto const lines(lua_tonumber(lua, -2));
    auto const height(lua_tonumber(lua, -1));
    event::moved(file, line, lines, height);
    return 0;
  }

  int enter(lua_State * const lua)
  {
    auto const file(lua_tostring(lua, -2));
    auto const data(lua_tostring(lua, -1));
    event::enter(file, data);
    return 0;
  }

  int destroy(lua_State * const lua)
  {
    auto const file(lua_tostring(lua, -1));
    event::destroy(file);
    return 0;
  }

  int last_error(lua_State * const lua)
  {
    lua_pushstring(lua, event::last_error().c_str());
    return 1;
  }

  int api_version(lua_State * const lua)
  {
    std::size_t constexpr const version{ 0x8c6648a };
    lua_pushinteger(lua, version);
    return 1;
  }
}

extern "C" int luaopen_color_coded(lua_State * const lua)
{
  color_coded::lua::state(lua);
  lua_register(lua, "color_coded_pull",
    (color_coded::safe_func<decltype(&color_coded::pull),
                            &color_coded::pull>()));
  lua_register(lua, "color_coded_push",
    (color_coded::safe_func<decltype(&color_coded::push),
                            &color_coded::push>()));
  lua_register(lua, "color_coded_moved",
    (color_coded::safe_func<decltype(&color_coded::moved),
                            &color_coded::moved>()));
  lua_register(lua, "color_coded_enter",
    (color_coded::safe_func<decltype(&color_coded::enter),
                            &color_coded::enter>()));
  lua_register(lua, "color_coded_destroy",
    (color_coded::safe_func<decltype(&color_coded::destroy),
                            &color_coded::destroy>()));
  lua_register(lua, "color_coded_last_error",
    (color_coded::safe_func<decltype(&color_coded::last_error),
                            &color_coded::last_error>()));
  lua_register(lua, "color_coded_api_version",
    (color_coded::safe_func<decltype(&color_coded::api_version),
                            &color_coded::api_version>()));
  return 0;
}
