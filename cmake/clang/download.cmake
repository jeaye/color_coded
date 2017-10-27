if(NOT CUSTOM_CLANG)
  set(CLANG_VERSION "5.0.0")
  message(STATUS "Locating trusted Clang/LLVM ${CLANG_VERSION}")

  set(CLANG_URL "http://llvm.org/releases/${CLANG_VERSION}" )
  if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CLANG_DIRNAME "clang+llvm-${CLANG_VERSION}-x86_64-apple-darwin")
    set(CLANG_SHA256 "326be172ccb61210c9ae5dced27204977e249ec6589521cc30f82fd0904b0671")
    set(CLANG_FILENAME "${CLANG_DIRNAME}.tar.xz")
  elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    if(64_BIT_PLATFORM)
      set(CLANG_DIRNAME "clang+llvm-${CLANG_VERSION}-linux-x86_64-ubuntu14.04")
      set(CLANG_SHA256 "58c1171f326108cfb7641441c5ede7846d58823bce3206c86a84c7ef7748860d")
      set(CLANG_FILENAME "${CLANG_DIRNAME}.tar.xz")
    else()
      message(FATAL_ERROR "No pre-built Clang ${CLANG_VERSION} binaries for 32 bit Linux; this system is not supported")
    endif()
  elseif(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
    if(64_BIT_PLATFORM)
      set(CLANG_DIRNAME "clang+llvm-${CLANG_VERSION}-amd64-unknown-freebsd10")
      set(CLANG_SHA256 "e55b646390da0a24e27f9761eecf0b31936483c9a3e84c12de0bb1a0d95bab6c")
      set(CLANG_FILENAME "${CLANG_DIRNAME}.tar.xz")
    else()
      set(CLANG_DIRNAME "clang+llvm-${CLANG_VERSION}-i386-unknown-freebsd10")
      set(CLANG_SHA256 "2ea32ad7cd30d8e849113747b5bfda8e6eb0fb2f9b01cbe9eb61e884c0bd69eb")
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
