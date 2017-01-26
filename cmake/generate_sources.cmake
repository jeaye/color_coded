message(STATUS "Generating sources")
execute_process(
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/lib/generate_sources ${LLVM_ROOT_DIR} ${LLVM_VERSION_STRING}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
