if(NOT CUSTOM_CLANG)
  set(CLANG_VERSION "7.0.0")
  message(STATUS "Locating trusted Clang/LLVM ${CLANG_VERSION}")

  set(CLANG_URL "https://llvm.org/releases/${CLANG_VERSION}" )
  if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CLANG_DIRNAME "clang+llvm-${CLANG_VERSION}-x86_64-apple-darwin")
    set(CLANG_SHA256 "b3ad93c3d69dfd528df9c5bb1a434367babb8f3baea47fbb99bf49f1b03c94ca")
    set(CLANG_FILENAME "${CLANG_DIRNAME}.tar.xz")
  elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    if(64_BIT_PLATFORM)
      set(CLANG_DIRNAME "clang+llvm-${CLANG_VERSION}-x86_64-linux-gnu-ubuntu-14.04")
      set(CLANG_SHA256 "5c90e61b06d37270bc26edb305d7e498e2c7be22d99e0afd9f2274ef5458575a")
      set(CLANG_FILENAME "${CLANG_DIRNAME}.tar.xz")
    else()
      message(FATAL_ERROR "No pre-built Clang ${CLANG_VERSION} binaries for 32 bit Linux; this system is not supported")
    endif()
  elseif(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
    if(64_BIT_PLATFORM)
      set(CLANG_DIRNAME "clang+llvm-${CLANG_VERSION}-amd64-unknown-freebsd11")
      set(CLANG_SHA256 "95ceb933ccf76e3ddaa536f41ab82c442bbac07cdea6f9fbf6e3b13cc1711255")
      set(CLANG_FILENAME "${CLANG_DIRNAME}.tar.xz")
    else()
      set(CLANG_DIRNAME "clang+llvm-${CLANG_VERSION}-i386-unknown-freebsd11")
      set(CLANG_SHA256 "35460d34a8b3d856e0d7c0b2b20d31f0d1ec05908c830a81f586721e8f8fb04f")
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
