cmake_minimum_required(VERSION 2.8)

project(color_coded)

set(CMAKE_BUILD_TYPE RELEASE)

# Shut up the warning about CMake policy CMP0042.
if(APPLE)
  set(CMAKE_MACOSX_RPATH ON)
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
  message(STATUS "Found 32bit system")
  set(64_BIT_PLATFORM 0)
else()
  message(STATUS "Found 64bit system")
  set(64_BIT_PLATFORM 1)
endif()

# C++14
include(cmake/cxx14.cmake)

# Lua
include(cmake/lua.cmake)

# Boost
include(cmake/boost.cmake)

# Curses
find_package(Curses REQUIRED)

# ZLib
find_package(ZLIB REQUIRED)

# Clang
include(cmake/clang/download.cmake)

include_directories(
  include

  ${LLVM_ROOT_PATH}/include

  ${CURSES_INCLUDE_PATH}
  ${ZLIB_INCLUDE_PATH}
)

include(cmake/generate_sources.cmake)

add_library(color_coded SHARED src/main.cpp)

add_custom_target(color_coded_track_api
  COMMAND ${CMAKE_CURRENT_LIST_DIR}/lib/track_api
  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)
add_dependencies(color_coded color_coded_track_api)

# Force the same naming across platforms
set_property(TARGET color_coded PROPERTY PREFIX "")
set_property(TARGET color_coded PROPERTY OUTPUT_NAME "color_coded.so")
set_property(TARGET color_coded PROPERTY SUFFIX "")

# Enable warnings
set_property(TARGET color_coded PROPERTY COMPILE_FLAGS "-Wall -Wextra -pedantic")

# Fix compilation problem: relocation R_X86_64_32 against `a local symbol' can not be
# used when making a shared object; recompile with -fPIC.
# See http://www.cmake.org/pipermail/cmake/2007-May/014350.html
if(64_BIT_PLATFORM)
  set_property(TARGET color_coded_boost PROPERTY COMPILE_FLAGS "-fPIC")
endif()

# Clang
include(cmake/clang/link.cmake)

target_link_libraries(color_coded
  color_coded_boost
  ${LUA_LIBRARIES}
  ${CURSES_LIBRARY}
  ${ZLIB_LIBRARIES}
)

# Install locally
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_LIST_DIR})
install(TARGETS color_coded DESTINATION bin)
