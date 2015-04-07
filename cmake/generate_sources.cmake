message("Generating sources ... ")
execute_process(
  COMMAND ${CMAKE_CURRENT_LIST_DIR}/lib/generate_sources ${CMAKE_CURRENT_LIST_DIR} ${LLVM_ROOT_PATH})
