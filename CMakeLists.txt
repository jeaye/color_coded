cmake_minimum_required(VERSION 2.9)

# Shut up the warning about CMake policy CMP0042.
if(APPLE)
  set(CMAKE_MACOSX_RPATH ON)
endif()

project(color_coded)

# C++14
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

# Lua
find_package(Lua REQUIRED)
if(${LUA_VERSION_STRING} VERSION_LESS "5.2")
  message(FATAL_ERROR "Lua 5.2 is required")
endif()

include_directories(${LUA_INCLUDE_DIR})

# Boost
include_directories(lib/boost)
set(BOOST_SRC
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
message("Downloading Clang/LLVM 3.6.0 ...")

# Check if platform is 64 bit
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
  set(64_BIT_PLATFORM 0)
else()
  set(64_BIT_PLATFORM 1)
endif()

set(CLANG_URL "http://llvm.org/releases/3.6.0" )
if(APPLE)
  set(CLANG_DIRNAME "clang+llvm-3.6.0-x86_64-apple-darwin")
  set(CLANG_MD5 "103e06006c88d391c9c369a4671e3363")
  set(CLANG_FILENAME "${CLANG_DIRNAME}.tar.xz")
else()
  if(64_BIT_PLATFORM)
    set(CLANG_DIRNAME "clang+llvm-3.6.0-x86_64-linux-gnu-ubuntu-14.04")
    set(CLANG_MD5 "cfb2ebc01573e666770b9c5f72deb04e")
    set(CLANG_FILENAME "${CLANG_DIRNAME}.tar.xz")
  else()
    message(WARNING "No pre-built Clang 3.6.0 binaries for 32 bit Linux, downloading Clang 3.3")
    set(CLANG_DIRNAME "clang+llvm-3.3-i386-debian6")
    set(CLANG_MD5 "415d033b60659433d4631df894673802")
    set(CLANG_FILENAME "${CLANG_DIRNAME}.tar.bz2")
  endif()
endif()

file(DOWNLOAD
  "${CLANG_URL}/${CLANG_FILENAME}" "./${CLANG_FILENAME}"
  SHOW_PROGRESS EXPECTED_MD5 "${CLANG_MD5}")

message("Extracting Clang/LLVM 3.6.0 ...")

if(CLANG_FILENAME MATCHES ".+bz2")
  execute_process(COMMAND tar -xjf ${CLANG_FILENAME})
elseif(CLANG_FILENAME MATCHES ".+xz")
  execute_process(COMMAND tar -xJf ${CLANG_FILENAME})
else()
  execute_process(COMMAND tar -xzf ${CLANG_FILENAME})
endif()

# We need to set PATH_TO_LLVM_ROOT. To do that, we first have to find the
# folder name the archive produced. It isn't the archive base name.
execute_process(COMMAND
  find ${CMAKE_CURRENT_BINARY_DIR} -maxdepth 1 -type d -name clang*
  OUTPUT_VARIABLE LLVM_ROOT_PATH
  OUTPUT_STRIP_TRAILING_WHITESPACE)

include_directories(
  include

  ${LLVM_ROOT_PATH}/include

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

message("Generating sources ... ")
execute_process(
  COMMAND ${CMAKE_CURRENT_LIST_DIR}/lib/generate_sources ${CMAKE_CURRENT_LIST_DIR} ${LLVM_ROOT_PATH})

add_library(boost_parts STATIC ${BOOST_SRC})

add_library(color_coded SHARED src/main.cpp)

# Force the same naming across platforms
set_property(TARGET color_coded PROPERTY PREFIX "")
set_property(TARGET color_coded PROPERTY OUTPUT_NAME "color_coded.so")
set_property(TARGET color_coded PROPERTY SUFFIX "")

# Clang
set(LLVM_LIB_PATH ${LLVM_ROOT_PATH}/lib)
target_link_libraries(color_coded
  boost_parts
  ${LUA_LIBRARIES}

  ${LLVM_LIB_PATH}/libclang.a
  ${LLVM_LIB_PATH}/libclangIndex.a
  ${LLVM_LIB_PATH}/libclangFormat.a
  ${LLVM_LIB_PATH}/libclangFrontendTool.a
  ${LLVM_LIB_PATH}/libclangFrontend.a
  ${LLVM_LIB_PATH}/libclangDriver.a
  ${LLVM_LIB_PATH}/libclangSerialization.a
  ${LLVM_LIB_PATH}/libclangCodeGen.a
  ${LLVM_LIB_PATH}/libclangParse.a
  ${LLVM_LIB_PATH}/libclangSema.a
  ${LLVM_LIB_PATH}/libclangStaticAnalyzerFrontend.a
  ${LLVM_LIB_PATH}/libclangStaticAnalyzerCheckers.a
  ${LLVM_LIB_PATH}/libclangStaticAnalyzerCore.a
  ${LLVM_LIB_PATH}/libclangAnalysis.a
  ${LLVM_LIB_PATH}/libclangARCMigrate.a
  ${LLVM_LIB_PATH}/libclangRewriteFrontend.a
  ${LLVM_LIB_PATH}/libclangRewrite.a
  ${LLVM_LIB_PATH}/libclangEdit.a
  ${LLVM_LIB_PATH}/libclangAST.a
  ${LLVM_LIB_PATH}/libclangASTMatchers.a
  ${LLVM_LIB_PATH}/libclangDynamicASTMatchers.a
  ${LLVM_LIB_PATH}/libclangLex.a
  ${LLVM_LIB_PATH}/libclangBasic.a
  ${LLVM_LIB_PATH}/libclangTooling.a
  ${LLVM_LIB_PATH}/libclangToolingCore.a
  ${LLVM_LIB_PATH}/libLLVMLTO.a
  ${LLVM_LIB_PATH}/libLLVMObjCARCOpts.a
  ${LLVM_LIB_PATH}/libLLVMLinker.a
  ${LLVM_LIB_PATH}/libLLVMBitWriter.a
  ${LLVM_LIB_PATH}/libLLVMIRReader.a
  ${LLVM_LIB_PATH}/libLLVMAsmParser.a
  ${LLVM_LIB_PATH}/libLLVMR600CodeGen.a
  ${LLVM_LIB_PATH}/libLLVMipo.a
  ${LLVM_LIB_PATH}/libLLVMVectorize.a
  ${LLVM_LIB_PATH}/libLLVMR600AsmParser.a
  ${LLVM_LIB_PATH}/libLLVMR600Desc.a
  ${LLVM_LIB_PATH}/libLLVMR600Info.a
  ${LLVM_LIB_PATH}/libLLVMR600AsmPrinter.a
  ${LLVM_LIB_PATH}/libLLVMSystemZDisassembler.a
  ${LLVM_LIB_PATH}/libLLVMSystemZCodeGen.a
  ${LLVM_LIB_PATH}/libLLVMSystemZAsmParser.a
  ${LLVM_LIB_PATH}/libLLVMSystemZDesc.a
  ${LLVM_LIB_PATH}/libLLVMSystemZInfo.a
  ${LLVM_LIB_PATH}/libLLVMSystemZAsmPrinter.a
  ${LLVM_LIB_PATH}/libLLVMHexagonDisassembler.a
  ${LLVM_LIB_PATH}/libLLVMHexagonCodeGen.a
  ${LLVM_LIB_PATH}/libLLVMHexagonDesc.a
  ${LLVM_LIB_PATH}/libLLVMHexagonInfo.a
  ${LLVM_LIB_PATH}/libLLVMNVPTXCodeGen.a
  ${LLVM_LIB_PATH}/libLLVMNVPTXDesc.a
  ${LLVM_LIB_PATH}/libLLVMNVPTXInfo.a
  ${LLVM_LIB_PATH}/libLLVMNVPTXAsmPrinter.a
  ${LLVM_LIB_PATH}/libLLVMCppBackendCodeGen.a
  ${LLVM_LIB_PATH}/libLLVMCppBackendInfo.a
  ${LLVM_LIB_PATH}/libLLVMMSP430CodeGen.a
  ${LLVM_LIB_PATH}/libLLVMMSP430Desc.a
  ${LLVM_LIB_PATH}/libLLVMMSP430Info.a
  ${LLVM_LIB_PATH}/libLLVMMSP430AsmPrinter.a
  ${LLVM_LIB_PATH}/libLLVMXCoreDisassembler.a
  ${LLVM_LIB_PATH}/libLLVMXCoreCodeGen.a
  ${LLVM_LIB_PATH}/libLLVMXCoreDesc.a
  ${LLVM_LIB_PATH}/libLLVMXCoreInfo.a
  ${LLVM_LIB_PATH}/libLLVMXCoreAsmPrinter.a
  ${LLVM_LIB_PATH}/libLLVMMipsDisassembler.a
  ${LLVM_LIB_PATH}/libLLVMMipsCodeGen.a
  ${LLVM_LIB_PATH}/libLLVMMipsAsmParser.a
  ${LLVM_LIB_PATH}/libLLVMMipsDesc.a
  ${LLVM_LIB_PATH}/libLLVMMipsInfo.a
  ${LLVM_LIB_PATH}/libLLVMMipsAsmPrinter.a
  ${LLVM_LIB_PATH}/libLLVMAArch64Disassembler.a
  ${LLVM_LIB_PATH}/libLLVMAArch64CodeGen.a
  ${LLVM_LIB_PATH}/libLLVMAArch64AsmParser.a
  ${LLVM_LIB_PATH}/libLLVMAArch64Desc.a
  ${LLVM_LIB_PATH}/libLLVMAArch64Info.a
  ${LLVM_LIB_PATH}/libLLVMAArch64AsmPrinter.a
  ${LLVM_LIB_PATH}/libLLVMAArch64Utils.a
  ${LLVM_LIB_PATH}/libLLVMARMDisassembler.a
  ${LLVM_LIB_PATH}/libLLVMARMCodeGen.a
  ${LLVM_LIB_PATH}/libLLVMARMAsmParser.a
  ${LLVM_LIB_PATH}/libLLVMARMDesc.a
  ${LLVM_LIB_PATH}/libLLVMARMInfo.a
  ${LLVM_LIB_PATH}/libLLVMARMAsmPrinter.a
  ${LLVM_LIB_PATH}/libLLVMPowerPCDisassembler.a
  ${LLVM_LIB_PATH}/libLLVMPowerPCCodeGen.a
  ${LLVM_LIB_PATH}/libLLVMPowerPCAsmParser.a
  ${LLVM_LIB_PATH}/libLLVMPowerPCDesc.a
  ${LLVM_LIB_PATH}/libLLVMPowerPCInfo.a
  ${LLVM_LIB_PATH}/libLLVMPowerPCAsmPrinter.a
  ${LLVM_LIB_PATH}/libLLVMSparcDisassembler.a
  ${LLVM_LIB_PATH}/libLLVMSparcCodeGen.a
  ${LLVM_LIB_PATH}/libLLVMSparcAsmParser.a
  ${LLVM_LIB_PATH}/libLLVMSparcDesc.a
  ${LLVM_LIB_PATH}/libLLVMSparcInfo.a
  ${LLVM_LIB_PATH}/libLLVMSparcAsmPrinter.a
  ${LLVM_LIB_PATH}/libLLVMTableGen.a
  ${LLVM_LIB_PATH}/libLLVMDebugInfo.a
  ${LLVM_LIB_PATH}/libLLVMOption.a
  ${LLVM_LIB_PATH}/libLLVMX86Disassembler.a
  ${LLVM_LIB_PATH}/libLLVMX86AsmParser.a
  ${LLVM_LIB_PATH}/libLLVMX86CodeGen.a
  ${LLVM_LIB_PATH}/libLLVMSelectionDAG.a
  ${LLVM_LIB_PATH}/libLLVMAsmPrinter.a
  ${LLVM_LIB_PATH}/libLLVMX86Desc.a
  ${LLVM_LIB_PATH}/libLLVMMCDisassembler.a
  ${LLVM_LIB_PATH}/libLLVMX86Info.a
  ${LLVM_LIB_PATH}/libLLVMX86AsmPrinter.a
  ${LLVM_LIB_PATH}/libLLVMX86Utils.a
  ${LLVM_LIB_PATH}/libLLVMMCJIT.a
  ${LLVM_LIB_PATH}/libLLVMLineEditor.a
  ${LLVM_LIB_PATH}/libLLVMInstrumentation.a
  ${LLVM_LIB_PATH}/libLLVMInterpreter.a
  ${LLVM_LIB_PATH}/libLLVMExecutionEngine.a
  ${LLVM_LIB_PATH}/libLLVMRuntimeDyld.a
  ${LLVM_LIB_PATH}/libLLVMCodeGen.a
  ${LLVM_LIB_PATH}/libLLVMScalarOpts.a
  ${LLVM_LIB_PATH}/libLLVMProfileData.a
  ${LLVM_LIB_PATH}/libLLVMObject.a
  ${LLVM_LIB_PATH}/libLLVMMCParser.a
  ${LLVM_LIB_PATH}/libLLVMBitReader.a
  ${LLVM_LIB_PATH}/libLLVMInstCombine.a
  ${LLVM_LIB_PATH}/libLLVMTransformUtils.a
  ${LLVM_LIB_PATH}/libLLVMipa.a
  ${LLVM_LIB_PATH}/libLLVMAnalysis.a
  ${LLVM_LIB_PATH}/libLLVMTarget.a
  ${LLVM_LIB_PATH}/libLLVMMC.a
  ${LLVM_LIB_PATH}/libLLVMCore.a
  ${LLVM_LIB_PATH}/libLLVMSupport.a

  ${CURSES_LIBRARY}
  ${ZLIB_LIBRARIES}
)

# Install locally
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_LIST_DIR})
install(TARGETS color_coded DESTINATION bin)
