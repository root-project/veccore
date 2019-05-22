set(CTEST_PROJECT_NAME "VecCore")

set(CTEST_SOURCE_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/..")
set(CTEST_BINARY_DIRECTORY "${CTEST_SOURCE_DIRECTORY}/build")

ctest_empty_binary_directory("${CTEST_BINARY_DIRECTORY}")

if(DEFINED ENV{CMAKE_GENERATOR})
  set(CTEST_CMAKE_GENERATOR $ENV{CMAKE_GENERATOR})
else()
  execute_process(COMMAND ${CMAKE_COMMAND} --system-information
    OUTPUT_VARIABLE CMAKE_SYSTEM_INFORMATION ERROR_VARIABLE ERROR)
  if(ERROR)
    message(FATAL_ERROR "Could not detect default CMake generator")
  endif()
  string(REGEX REPLACE ".+CMAKE_GENERATOR \"([-0-9A-Za-z ]+)\".*$" "\\1"
    CTEST_CMAKE_GENERATOR "${CMAKE_SYSTEM_INFORMATION}")
endif()

set(CMAKE_ARGS
  -DBUILD_BENCHMARKS=ON
  -DBUILD_TESTING=ON
  -DBUILD_UMESIMD=${UNIX}
  -DBUILD_VC=${UNIX}
  $ENV{CMAKE_ARGS}
  ${CMAKE_ARGS}
)

ctest_start(Continuous)
ctest_configure(OPTIONS "${CMAKE_ARGS}")
ctest_build()
ctest_test()
