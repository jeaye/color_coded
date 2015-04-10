include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
CHECK_CXX_COMPILER_FLAG("-std=c++1y" COMPILER_SUPPORTS_CXX1Y)
if(COMPILER_SUPPORTS_CXX14)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
elseif(COMPILER_SUPPORTS_CXX1Y)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
else()
  message(FATAL_ERROR "${CMAKE_CXX_COMPILER} has no C++14 support")
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
  exec_program(
    ${CMAKE_CXX_COMPILER}
    ARGS --version
    OUTPUT_VARIABLE _compiler_output)
  string(REGEX REPLACE ".* ([0-9]\\.[0-9]\\.[0-9]) .*" "\\1" GCC_COMPILER_VERSION ${_compiler_output})
  message(STATUS "C++ compiler version: ${gcc_compiler_version} [${CMAKE_CXX_COMPILER}]")

  if(${GCC_COMPILER_VERSION} VERSION_LESS "4.9")
    message(FATAL_ERROR "GCC 4.9 or greater is required; you have GCC ${GCC_COMPILER_VERSION}")
  endif()
endif()
