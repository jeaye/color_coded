#pragma once

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <string>

namespace color_coded
{
  namespace lua
  {
    lua_State* state(lua_State * const lua = nullptr)
    {
      static lua_State *lua_{};
      if(lua)
      { lua_ = lua; }
      return lua_;
    }

    void eval(std::string const &str)
    { luaL_dostring(state(), str.c_str()); }
  }
}
