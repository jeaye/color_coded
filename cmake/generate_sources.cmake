message(STATUS "Generating sources")
execute_process(
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/lib/generate_sources
          ${CMAKE_CURRENT_SOURCE_DIR} ${LLVM_ROOT_PATH}
)
