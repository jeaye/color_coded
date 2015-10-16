message(STATUS "Checking for luajit")

execute_process(
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/lib/check_luajit
  OUTPUT_QUIET ERROR_QUIET
  RESULT_VARIABLE RESULT
)

if(RESULT EQUAL 0)
  message(FATAL_ERROR "luajit doesn't support being embedded; to use color_coded, you must install vim without luajit")
endif()
