# Detect Clang libraries
#
# Defines the following variables:
#  CLANG_FOUND                 - True if Clang was found
#  CLANG_INCLUDE_DIRS          - Where to find Clang includes
#  CLANG_LIBRARY_DIRS          - Where to find Clang libraries
#
#  CLANG_LIBCLANG_LIB          - Libclang C library
#
#  CLANG_CLANGFRONTEND_LIB     - Clang Frontend (C++) Library
#  CLANG_CLANGDRIVER_LIB       - Clang Driver (C++) Library
#
#  CLANG_SEARCH_DIRS           - Where to search for Clang libraries
#  ...
#
#  CLANG_LIBS                  - All the Clang C++ libraries
#
# Uses the same include and library paths detected by FindLLVM.cmake
#
# See http://clang.llvm.org/docs/InternalsManual.html for full list of libraries

#=============================================================================
# Copyright 2014-2015 Kevin Funk <kfunk@kde.org>
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.

#=============================================================================

set(CLANG_FOUND FALSE)

if (LLVM_FOUND AND LLVM_LIBRARY_DIRS)
  MACRO(FIND_AND_ADD_CLANG_LIB _libname_)
  find_library(CLANG_${_libname_}_LIB ${_libname_} HINTS ${LLVM_LIBRARY_DIRS})
  if (CLANG_${_libname_}_LIB)
     set(CLANG_LIBS ${CLANG_LIBS} ${CLANG_${_libname_}_LIB})
  endif (CLANG_${_libname_}_LIB)
  ENDMACRO(FIND_AND_ADD_CLANG_LIB)

  # note: On Windows there's 'libclang.dll' instead of 'clang.dll' -> search for 'libclang', too
  find_library(CLANG_LIBCLANG_LIB NAMES clang libclang HINTS ${LLVM_LIBRARY_DIRS}) # LibClang: high-level C interface

  FIND_AND_ADD_CLANG_LIB(clang)
  FIND_AND_ADD_CLANG_LIB(clangTooling)
  FIND_AND_ADD_CLANG_LIB(clangToolingCore)
  FIND_AND_ADD_CLANG_LIB(clangIndex)
  FIND_AND_ADD_CLANG_LIB(clangFrontend)
  FIND_AND_ADD_CLANG_LIB(clangFrontendTool)
  FIND_AND_ADD_CLANG_LIB(clangDriver)
  FIND_AND_ADD_CLANG_LIB(clangFormat)
  FIND_AND_ADD_CLANG_LIB(clangCodeGen)
  FIND_AND_ADD_CLANG_LIB(clangParse)
  FIND_AND_ADD_CLANG_LIB(clangSema)
  FIND_AND_ADD_CLANG_LIB(clangEdit)
  FIND_AND_ADD_CLANG_LIB(clangChecker)
  FIND_AND_ADD_CLANG_LIB(clangAnalysis)
  FIND_AND_ADD_CLANG_LIB(clangSerialization)
  FIND_AND_ADD_CLANG_LIB(clangStaticAnalyzerFrontend)
  FIND_AND_ADD_CLANG_LIB(clangStaticAnalyzerCheckers)
  FIND_AND_ADD_CLANG_LIB(clangStaticAnalyzerCore)
  FIND_AND_ADD_CLANG_LIB(clangRewriteFrontend)
  FIND_AND_ADD_CLANG_LIB(clangRewrite)
  FIND_AND_ADD_CLANG_LIB(clangAnalysis)
  FIND_AND_ADD_CLANG_LIB(clangArcMigrate)
  FIND_AND_ADD_CLANG_LIB(clangAST)
  FIND_AND_ADD_CLANG_LIB(clangASTMatchers)
  FIND_AND_ADD_CLANG_LIB(clangDynamicASTMatchers)
  FIND_AND_ADD_CLANG_LIB(clangLex)
  FIND_AND_ADD_CLANG_LIB(clangBasic)

  find_path(CLANG_INCLUDE_DIRS clang/Basic/Version.h HINTS ${LLVM_INCLUDE_DIRS})
else()
  message(STATUS "Either LLVM (${LLVM_FOUND}) and LLVM_LIBRARY_DIRS (${LLVM_LIBRARY_DIRS}) not set")
endif()

if(CLANG_LIBS OR CLANG_LIBCLANG_LIB)
  set(CLANG_FOUND TRUE)
else()
  message(STATUS "Could not find any Clang libraries in ${LLVM_LIBRARY_DIRS}")
endif()

if(CLANG_FOUND)
  set(CLANG_LIBRARY_DIRS ${LLVM_LIBRARY_DIRS})
  set(CLANG_INCLUDE_DIRS ${LLVM_INCLUDE_DIRS})

  # check whether llvm-config comes from an install prefix
  execute_process(
    COMMAND ${LLVM_CONFIG_EXECUTABLE} --src-root
    OUTPUT_VARIABLE _llvmSourceRoot
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  string(FIND "${LLVM_INCLUDE_DIRS}" "${_llvmSourceRoot}" _llvmIsInstalled)
  if (NOT _llvmIsInstalled)
    message(STATUS "Detected that llvm-config comes from a build-tree, adding more include directories for Clang")
    list(APPEND CLANG_INCLUDE_DIRS
         "${LLVM_INSTALL_PREFIX}/tools/clang/include" # build dir
         "${_llvmSourceRoot}/tools/clang/include"     # source dir
    )
  endif()

  message(STATUS "Found Clang (LLVM version: ${LLVM_VERSION_STRING})")
  message(STATUS "  Include dirs:       ${CLANG_INCLUDE_DIRS}")
  message(STATUS "  Clang libraries:    ${CLANG_LIBS}")
  message(STATUS "  Libclang C library: ${CLANG_LIBCLANG_LIB}")
else()
  if(Clang_FIND_REQUIRED)
    message(FATAL_ERROR "Could NOT find Clang")
  endif()
endif()
