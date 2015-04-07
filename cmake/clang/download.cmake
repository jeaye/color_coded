set(CLANG_VERSION "3.6.0")
message("Downloading Clang/LLVM ${CLANG_VERSION} ...")

set(CLANG_URL "http://llvm.org/releases/${CLANG_VERSION}" )
if(APPLE)
  set(CLANG_DIRNAME "clang+llvm-${CLANG_VERSION}-x86_64-apple-darwin")
  set(CLANG_MD5 "103e06006c88d391c9c369a4671e3363")
  set(CLANG_FILENAME "${CLANG_DIRNAME}.tar.xz")
else()
  if(64_BIT_PLATFORM)
    set(CLANG_DIRNAME "clang+llvm-${CLANG_VERSION}-x86_64-linux-gnu-ubuntu-14.04")
    set(CLANG_MD5 "cfb2ebc01573e666770b9c5f72deb04e")
    set(CLANG_FILENAME "${CLANG_DIRNAME}.tar.xz")
  else()
    message(FATAL_ERROR "No pre-built Clang ${CLANG_VERSION} binaries for 32 bit Linux; this system is not supported")
  endif()
endif()

file(DOWNLOAD
  "${CLANG_URL}/${CLANG_FILENAME}" "./${CLANG_FILENAME}"
  SHOW_PROGRESS EXPECTED_MD5 "${CLANG_MD5}")

if(NOT EXISTS ${CLANG_DIRNAME})
  message("Extracting Clang/LLVM ${CLANG_VERSION} ...")

  if(CLANG_FILENAME MATCHES ".+bz2")
    execute_process(COMMAND tar -xjf ${CLANG_FILENAME} -C ${CLANG_DIRNAME} --strip-components 1)
  elseif(CLANG_FILENAME MATCHES ".+xz")
    execute_process(COMMAND tar -xJf ${CLANG_FILENAME} -C ${CLANG_DIRNAME} --strip-components 1)
  else()
    execute_process(COMMAND tar -xzf ${CLANG_FILENAME} -C ${CLANG_DIRNAME} --strip-components 1)
  endif()
else()
  message("Clang/LLVM ${CLANG_VERSION} already extracted")
endif()

get_filename_component(LLVM_ROOT_PATH ${CLANG_DIRNAME} ABSOLUTE)
