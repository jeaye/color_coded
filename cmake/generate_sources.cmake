message(STATUS "Generating sources")
execute_process(
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/lib/generate_sources ${LLVM_ROOT_DIR} ${CLANG_VERSION}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
