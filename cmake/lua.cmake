find_package(Lua52 QUIET)
if(NOT ${LUA_FOUND})
  find_package(Lua51 QUIET)
  if(NOT ${LUA_FOUND})
    find_package(Lua QUIET)

    if(NOT ${LUA_FOUND})
      message(FATAL_ERROR "Lua is required")
    endif()
  endif()
endif()

include_directories(${LUA_INCLUDE_DIR})
