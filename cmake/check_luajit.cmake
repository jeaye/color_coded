message(STATUS "Checking for luajit")

execute_process(
  COMMAND vim --version | grep "luajit"
  OUTPUT_QUIET ERROR_QUIET
  RESULT_VARIABLE RESULT
)

if(RESULT EQUAL 1)
  message(FATAL_ERROR "luajit doesn't support being embedded; to use color_coded, you must install vim without luajit")
endif()
