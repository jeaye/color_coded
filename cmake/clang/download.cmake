if(NOT CUSTOM_CLANG)
  set(CLANG_VERSION "4.0.0")
  message(STATUS "Locating trusted Clang/LLVM ${CLANG_VERSION}")

  set(CLANG_URL "http://llvm.org/releases/${CLANG_VERSION}" )
  if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CLANG_DIRNAME "clang+llvm-${CLANG_VERSION}-x86_64-apple-darwin")
    set(CLANG_SHA256 "5504064916d0651c185ceff85871298f31e2eaf4abf1333b2c36f5eed5e9cde6")
    set(CLANG_FILENAME "${CLANG_DIRNAME}.tar.xz")
  elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    if(64_BIT_PLATFORM)
      set(CLANG_DIRNAME "clang+llvm-${CLANG_VERSION}-x86_64-linux-gnu-ubuntu-14.04")
      set(CLANG_SHA256 "1d15b6337ffc0876ed1a9827cae566e24639e0f5d7d186b2de04c38d762336b4")
      set(CLANG_FILENAME "${CLANG_DIRNAME}.tar.xz")
    else()
      message(FATAL_ERROR "No pre-built Clang ${CLANG_VERSION} binaries for 32 bit Linux; this system is not supported")
    endif()
  elseif(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
    if(64_BIT_PLATFORM)
      set(CLANG_DIRNAME "clang+llvm-${CLANG_VERSION}-amd64-unknown-freebsd10")
      set(CLANG_SHA256 "e8878b589c2558aeb9733661e9a23fd47d41be0dc200aa50acf7a7eb1ab53eee")
      set(CLANG_FILENAME "${CLANG_DIRNAME}.tar.xz")
    else()
      set(CLANG_DIRNAME "clang+llvm-${CLANG_VERSION}-i386-unknown-freebsd10")
      set(CLANG_SHA256 "f3bfd0e4778d5e6b94f774e4531a61a662c7688e08b54cef5e5fc53711e65243")
      set(CLANG_FILENAME "${CLANG_DIRNAME}.tar.xz")
    endif()
  endif()

  file(DOWNLOAD
    "${CLANG_URL}/${CLANG_FILENAME}" "./${CLANG_FILENAME}"
    SHOW_PROGRESS EXPECTED_HASH SHA256=${CLANG_SHA256})

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

  set(LLVM_ROOT_DIR ${CMAKE_CURRENT_BINARY_DIR}/${CLANG_DIRNAME})
  message(STATUS "Clang/LLVM root path: ${LLVM_ROOT_DIR}")
else()
  message(STATUS "Trusting custom clang at ${LLVM_ROOT_DIR}")
endif()

set(LLVM_LIB_PATH ${LLVM_ROOT_DIR}/lib)

if(APPLE)
  execute_process(COMMAND ln -sf ${LLVM_LIB_PATH}/libclang.dylib ${LLVM_LIB_PATH}/libclang.so)
endif()

add_custom_target(clean_clang
  COMMAND rm -rf ${CLANG_FILENAME}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
