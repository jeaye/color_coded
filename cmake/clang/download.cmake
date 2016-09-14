if(NOT CUSTOM_CLANG)
  set(CLANG_VERSION "3.8.0")
  message(STATUS "Locating trusted Clang/LLVM ${CLANG_VERSION}")

  set(CLANG_URL "http://llvm.org/releases/${CLANG_VERSION}" )
  if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CLANG_DIRNAME "clang+llvm-${CLANG_VERSION}-x86_64-apple-darwin")
    set(CLANG_MD5 "9063820534130f4d941b1881c70fb3bb")
    set(CLANG_FILENAME "${CLANG_DIRNAME}.tar.xz")
  elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    if(64_BIT_PLATFORM)
      set(CLANG_DIRNAME "clang+llvm-${CLANG_VERSION}-x86_64-linux-gnu-ubuntu-14.04")
      set(CLANG_MD5 "890274421f4480ce228ed3eab1fd3c73")
      set(CLANG_FILENAME "${CLANG_DIRNAME}.tar.xz")
    else()
      message(FATAL_ERROR "No pre-built Clang ${CLANG_VERSION} binaries for 32 bit Linux; this system is not supported")
    endif()
  elseif(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
    if(64_BIT_PLATFORM)
      set(CLANG_DIRNAME "clang+llvm-${CLANG_VERSION}-amd64-unknown-freebsd10")
      set(CLANG_MD5 "1f6fd3a1fc3a2e24ecdacb072e9f7212")
      set(CLANG_FILENAME "${CLANG_DIRNAME}.tar.xz")
    else()
      set(CLANG_DIRNAME "clang+llvm-${CLANG_VERSION}-i386-unknown-freebsd10")
      set(CLANG_MD5 "08ccedd6d45ce1a4a45eda24d0ca52aa")
      set(CLANG_FILENAME "${CLANG_DIRNAME}.tar.xz")
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
