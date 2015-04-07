find_package(Lua REQUIRED)
if(${LUA_VERSION_STRING} VERSION_LESS "5.2")
  message(FATAL_ERROR "Lua 5.2 is required")
endif()

include_directories(${LUA_INCLUDE_DIR})
