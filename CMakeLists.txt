cmake_minimum_required(VERSION 2.6)

project(color_coded)

set(color_coded_VERSION_MAJOR 0)
set(color_coded_VERSION_MINOR 1)

# C++14
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
CHECK_CXX_COMPILER_FLAG("-std=c++1y" COMPILER_SUPPORTS_CXX1Y)
if(COMPILER_SUPPORTS_CXX14)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
elseif(COMPILER_SUPPORTS_CXX1Y)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
else()
  message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++14 support. Please use a different C++ compiler.")
endif()

# Lua
find_package(Lua REQUIRED)
include_directories(${LUA_INCLUDE_DIR})

# Boost
include_directories(lib/boost)
set(color_coded_BOOST_SRC
  lib/boost/filesystem/src/codecvt_error_category.cpp
  lib/boost/filesystem/src/operations.cpp
  lib/boost/filesystem/src/path.cpp
  lib/boost/filesystem/src/path_traits.cpp
  lib/boost/filesystem/src/portability.cpp
  lib/boost/filesystem/src/unique_path.cpp
  lib/boost/filesystem/src/utf8_codecvt_facet.cpp
  lib/boost/system/src/error_code.cpp
)

# Curses
find_package(Curses REQUIRED)

# ZLib
find_package(ZLIB REQUIRED)

# Clang
include_directories(
  include

  clang+llvm-3.6.0-x86_64-apple-darwin/include

  lib/boost/config/include
  lib/boost/core/include
  lib/boost/detail/include
  lib/boost/exception/include
  lib/boost/filesystem/include
  lib/boost/functional/include
  lib/boost/include
  lib/boost/io/include
  lib/boost/iterator/include
  lib/boost/mpl/include
  lib/boost/predef/include
  lib/boost/preprocessor/include
  lib/boost/range/include
  lib/boost/smart_ptr/include
  lib/boost/system/include
  lib/boost/type_traits/include
  lib/boost/utility/include

  ${CURSES_INCLUDE_PATH}
  ${ZLIB_INCLUDE_PATH}
)

add_library(boost_parts STATIC ${color_coded_BOOST_SRC})

add_library(color_coded SHARED src/main.cpp)

# Force the same naming across platforms
set_property(TARGET color_coded PROPERTY PREFIX "")
set_property(TARGET color_coded PROPERTY OUTPUT_NAME "color_coded.so")
set_property(TARGET color_coded PROPERTY SUFFIX "")

# Clang
set(color_coded_CLANG_LIB ${CMAKE_CURRENT_SOURCE_DIR}/clang+llvm-3.6.0-x86_64-apple-darwin/lib)
target_link_libraries(
  color_coded

  boost_parts
  ${LUA_LIBRARIES}
  ${color_coded_CLANG_LIB}/libclang.a
  ${color_coded_CLANG_LIB}/libclangIndex.a
  ${color_coded_CLANG_LIB}/libclangFormat.a
  ${color_coded_CLANG_LIB}/libclangFrontendTool.a
  ${color_coded_CLANG_LIB}/libclangFrontend.a
  ${color_coded_CLANG_LIB}/libclangDriver.a
  ${color_coded_CLANG_LIB}/libclangSerialization.a
  ${color_coded_CLANG_LIB}/libclangCodeGen.a
  ${color_coded_CLANG_LIB}/libclangParse.a
  ${color_coded_CLANG_LIB}/libclangSema.a
  ${color_coded_CLANG_LIB}/libclangStaticAnalyzerFrontend.a
  ${color_coded_CLANG_LIB}/libclangStaticAnalyzerCheckers.a
  ${color_coded_CLANG_LIB}/libclangStaticAnalyzerCore.a
  ${color_coded_CLANG_LIB}/libclangAnalysis.a
  ${color_coded_CLANG_LIB}/libclangARCMigrate.a
  ${color_coded_CLANG_LIB}/libclangRewriteFrontend.a
  ${color_coded_CLANG_LIB}/libclangRewrite.a
  ${color_coded_CLANG_LIB}/libclangEdit.a
  ${color_coded_CLANG_LIB}/libclangAST.a
  ${color_coded_CLANG_LIB}/libclangASTMatchers.a
  ${color_coded_CLANG_LIB}/libclangDynamicASTMatchers.a
  ${color_coded_CLANG_LIB}/libclangLex.a
  ${color_coded_CLANG_LIB}/libclangBasic.a
  ${color_coded_CLANG_LIB}/libclangTooling.a
  ${color_coded_CLANG_LIB}/libclangToolingCore.a
  ${color_coded_CLANG_LIB}/libLLVMLTO.a
  ${color_coded_CLANG_LIB}/libLLVMObjCARCOpts.a
  ${color_coded_CLANG_LIB}/libLLVMLinker.a
  ${color_coded_CLANG_LIB}/libLLVMBitWriter.a
  ${color_coded_CLANG_LIB}/libLLVMIRReader.a
  ${color_coded_CLANG_LIB}/libLLVMAsmParser.a
  ${color_coded_CLANG_LIB}/libLLVMR600CodeGen.a
  ${color_coded_CLANG_LIB}/libLLVMipo.a
  ${color_coded_CLANG_LIB}/libLLVMVectorize.a
  ${color_coded_CLANG_LIB}/libLLVMR600AsmParser.a
  ${color_coded_CLANG_LIB}/libLLVMR600Desc.a
  ${color_coded_CLANG_LIB}/libLLVMR600Info.a
  ${color_coded_CLANG_LIB}/libLLVMR600AsmPrinter.a
  ${color_coded_CLANG_LIB}/libLLVMSystemZDisassembler.a
  ${color_coded_CLANG_LIB}/libLLVMSystemZCodeGen.a
  ${color_coded_CLANG_LIB}/libLLVMSystemZAsmParser.a
  ${color_coded_CLANG_LIB}/libLLVMSystemZDesc.a
  ${color_coded_CLANG_LIB}/libLLVMSystemZInfo.a
  ${color_coded_CLANG_LIB}/libLLVMSystemZAsmPrinter.a
  ${color_coded_CLANG_LIB}/libLLVMHexagonDisassembler.a
  ${color_coded_CLANG_LIB}/libLLVMHexagonCodeGen.a
  ${color_coded_CLANG_LIB}/libLLVMHexagonDesc.a
  ${color_coded_CLANG_LIB}/libLLVMHexagonInfo.a
  ${color_coded_CLANG_LIB}/libLLVMNVPTXCodeGen.a
  ${color_coded_CLANG_LIB}/libLLVMNVPTXDesc.a
  ${color_coded_CLANG_LIB}/libLLVMNVPTXInfo.a
  ${color_coded_CLANG_LIB}/libLLVMNVPTXAsmPrinter.a
  ${color_coded_CLANG_LIB}/libLLVMCppBackendCodeGen.a
  ${color_coded_CLANG_LIB}/libLLVMCppBackendInfo.a
  ${color_coded_CLANG_LIB}/libLLVMMSP430CodeGen.a
  ${color_coded_CLANG_LIB}/libLLVMMSP430Desc.a
  ${color_coded_CLANG_LIB}/libLLVMMSP430Info.a
  ${color_coded_CLANG_LIB}/libLLVMMSP430AsmPrinter.a
  ${color_coded_CLANG_LIB}/libLLVMXCoreDisassembler.a
  ${color_coded_CLANG_LIB}/libLLVMXCoreCodeGen.a
  ${color_coded_CLANG_LIB}/libLLVMXCoreDesc.a
  ${color_coded_CLANG_LIB}/libLLVMXCoreInfo.a
  ${color_coded_CLANG_LIB}/libLLVMXCoreAsmPrinter.a
  ${color_coded_CLANG_LIB}/libLLVMMipsDisassembler.a
  ${color_coded_CLANG_LIB}/libLLVMMipsCodeGen.a
  ${color_coded_CLANG_LIB}/libLLVMMipsAsmParser.a
  ${color_coded_CLANG_LIB}/libLLVMMipsDesc.a
  ${color_coded_CLANG_LIB}/libLLVMMipsInfo.a
  ${color_coded_CLANG_LIB}/libLLVMMipsAsmPrinter.a
  ${color_coded_CLANG_LIB}/libLLVMAArch64Disassembler.a
  ${color_coded_CLANG_LIB}/libLLVMAArch64CodeGen.a
  ${color_coded_CLANG_LIB}/libLLVMAArch64AsmParser.a
  ${color_coded_CLANG_LIB}/libLLVMAArch64Desc.a
  ${color_coded_CLANG_LIB}/libLLVMAArch64Info.a
  ${color_coded_CLANG_LIB}/libLLVMAArch64AsmPrinter.a
  ${color_coded_CLANG_LIB}/libLLVMAArch64Utils.a
  ${color_coded_CLANG_LIB}/libLLVMARMDisassembler.a
  ${color_coded_CLANG_LIB}/libLLVMARMCodeGen.a
  ${color_coded_CLANG_LIB}/libLLVMARMAsmParser.a
  ${color_coded_CLANG_LIB}/libLLVMARMDesc.a
  ${color_coded_CLANG_LIB}/libLLVMARMInfo.a
  ${color_coded_CLANG_LIB}/libLLVMARMAsmPrinter.a
  ${color_coded_CLANG_LIB}/libLLVMPowerPCDisassembler.a
  ${color_coded_CLANG_LIB}/libLLVMPowerPCCodeGen.a
  ${color_coded_CLANG_LIB}/libLLVMPowerPCAsmParser.a
  ${color_coded_CLANG_LIB}/libLLVMPowerPCDesc.a
  ${color_coded_CLANG_LIB}/libLLVMPowerPCInfo.a
  ${color_coded_CLANG_LIB}/libLLVMPowerPCAsmPrinter.a
  ${color_coded_CLANG_LIB}/libLLVMSparcDisassembler.a
  ${color_coded_CLANG_LIB}/libLLVMSparcCodeGen.a
  ${color_coded_CLANG_LIB}/libLLVMSparcAsmParser.a
  ${color_coded_CLANG_LIB}/libLLVMSparcDesc.a
  ${color_coded_CLANG_LIB}/libLLVMSparcInfo.a
  ${color_coded_CLANG_LIB}/libLLVMSparcAsmPrinter.a
  ${color_coded_CLANG_LIB}/libLLVMTableGen.a
  ${color_coded_CLANG_LIB}/libLLVMDebugInfo.a
  ${color_coded_CLANG_LIB}/libLLVMOption.a
  ${color_coded_CLANG_LIB}/libLLVMX86Disassembler.a
  ${color_coded_CLANG_LIB}/libLLVMX86AsmParser.a
  ${color_coded_CLANG_LIB}/libLLVMX86CodeGen.a
  ${color_coded_CLANG_LIB}/libLLVMSelectionDAG.a
  ${color_coded_CLANG_LIB}/libLLVMAsmPrinter.a
  ${color_coded_CLANG_LIB}/libLLVMX86Desc.a
  ${color_coded_CLANG_LIB}/libLLVMMCDisassembler.a
  ${color_coded_CLANG_LIB}/libLLVMX86Info.a
  ${color_coded_CLANG_LIB}/libLLVMX86AsmPrinter.a
  ${color_coded_CLANG_LIB}/libLLVMX86Utils.a
  ${color_coded_CLANG_LIB}/libLLVMMCJIT.a
  ${color_coded_CLANG_LIB}/libLLVMLineEditor.a
  ${color_coded_CLANG_LIB}/libLLVMInstrumentation.a
  ${color_coded_CLANG_LIB}/libLLVMInterpreter.a
  ${color_coded_CLANG_LIB}/libLLVMExecutionEngine.a
  ${color_coded_CLANG_LIB}/libLLVMRuntimeDyld.a
  ${color_coded_CLANG_LIB}/libLLVMCodeGen.a
  ${color_coded_CLANG_LIB}/libLLVMScalarOpts.a
  ${color_coded_CLANG_LIB}/libLLVMProfileData.a
  ${color_coded_CLANG_LIB}/libLLVMObject.a
  ${color_coded_CLANG_LIB}/libLLVMMCParser.a
  ${color_coded_CLANG_LIB}/libLLVMBitReader.a
  ${color_coded_CLANG_LIB}/libLLVMInstCombine.a
  ${color_coded_CLANG_LIB}/libLLVMTransformUtils.a
  ${color_coded_CLANG_LIB}/libLLVMipa.a
  ${color_coded_CLANG_LIB}/libLLVMAnalysis.a
  ${color_coded_CLANG_LIB}/libLLVMTarget.a
  ${color_coded_CLANG_LIB}/libLLVMMC.a
  ${color_coded_CLANG_LIB}/libLLVMCore.a
  ${color_coded_CLANG_LIB}/libLLVMSupport.a

  ${CURSES_LIBRARY}
  ${ZLIB_LIBRARIES}
)

set(CMAKE_INSTALL_PREFIX "./")
install(TARGETS color_coded DESTINATION bin)
