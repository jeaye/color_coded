message("Downloading Clang/LLVM 3.6.0 ...")

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
    message(FATAL_ERROR "No pre-built Clang 3.6.0 binaries for 32 bit Linux; this system is not supported")
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
