message(STATUS "Generating sources")
execute_process(
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/lib/generate_sources ${LLVM_ROOT_PATH}
  WORKING_DIRECTORY $ENV{PWD}
)
