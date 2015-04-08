set(CLANG_VERSION "3.6.0")
message(STATUS "Locating trusted Clang/LLVM ${CLANG_VERSION}")

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

message(STATUS "Found ${CLANG_FILENAME}")

if(NOT EXISTS ${CLANG_DIRNAME})
  message(STATUS "Extracting Clang/LLVM ${CLANG_VERSION}")

  execute_process(COMMAND mkdir -p ${CLANG_DIRNAME})
  if(CLANG_FILENAME MATCHES ".+bz2")
    execute_process(COMMAND tar -xjf ${CLANG_FILENAME} -C ${CLANG_DIRNAME} --strip-components 1)
  elseif(CLANG_FILENAME MATCHES ".+xz")
    execute_process(COMMAND tar -xJf ${CLANG_FILENAME} -C ${CLANG_DIRNAME} --strip-components 1)
  else()
    execute_process(COMMAND tar -xzf ${CLANG_FILENAME} -C ${CLANG_DIRNAME} --strip-components 1)
  endif()
else()
  message(STATUS "Clang/LLVM ${CLANG_VERSION} already extracted")
endif()

set(LLVM_ROOT_PATH ${CMAKE_CURRENT_BINARY_DIR}/${CLANG_DIRNAME})
