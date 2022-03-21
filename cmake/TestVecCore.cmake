set(CTEST_PROJECT_NAME "VecCore")

set(CTEST_DROP_METHOD "http")
set(CTEST_DROP_SITE "cdash.cern.ch")
set(CTEST_DROP_LOCATION "/submit.php?project=VecCore")
set(CTEST_DROP_SITE_CDASH TRUE)

set(ENV{LANG} "C")
set(ENV{LC_ALL} "C")

set(CTEST_SOURCE_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/..")
set(CTEST_BINARY_DIRECTORY "${CTEST_SOURCE_DIRECTORY}/build")

ctest_empty_binary_directory("${CTEST_BINARY_DIRECTORY}")

if(MEMCHECK)
  find_program(CTEST_MEMORYCHECK_COMMAND NAMES valgrind)
endif()

if(NOT DEFINED CTEST_SITE)
  if(DEFINED ENV{BUILD_SITE})
    set(CTEST_SITE $ENV{BUILD_SITE})
  else()
    site_name(CTEST_SITE)
  endif()
endif()

if(NOT DEFINED CTEST_CONFIGURATION_TYPE)
  set(CTEST_CONFIGURATION_TYPE RelWithDebInfo)
endif()

if(PARALLEL)
  cmake_host_system_information(RESULT
    NCORES QUERY NUMBER_OF_PHYSICAL_CORES)
  if(NOT DEFINED ENV{CMAKE_BUILD_PARALLEL_LEVEL})
    set(ENV{CMAKE_BUILD_PARALLEL_LEVEL} ${NCORES})
  endif()
  if(NOT DEFINED ENV{CTEST_PARALLEL_LEVEL})
    set(ENV{CTEST_PARALLEL_LEVEL} ${NCORES})
  endif()
endif()

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

if(NOT DEFINED CTEST_BUILD_NAME)
  execute_process(COMMAND ${CMAKE_COMMAND} --system-information
    OUTPUT_VARIABLE CMAKE_SYSTEM_INFORMATION ERROR_VARIABLE ERROR)
  if(ERROR)
    message(FATAL_ERROR "Could not detect default configuration")
  endif()
  string(REGEX REPLACE ".+CMAKE_CXX_COMPILER_ID \"([-0-9A-Za-z ]+)\".*$" "\\1"
    COMPILER_ID "${CMAKE_SYSTEM_INFORMATION}")
  string(REGEX REPLACE ".+CMAKE_CXX_COMPILER_VERSION \"([^\"]+)\".*$" "\\1"
    COMPILER_VERSION "${CMAKE_SYSTEM_INFORMATION}")
  set(CTEST_BUILD_NAME "${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_PROCESSOR}")
  set(CTEST_BUILD_NAME "${CTEST_BUILD_NAME} ${COMPILER_ID} ${COMPILER_VERSION}")
  set(CTEST_BUILD_NAME "${CTEST_BUILD_NAME} ${CTEST_CONFIGURATION_TYPE}")
  set(CTEST_BUILD_NAME "${CTEST_BUILD_NAME} ${CTEST_SCRIPT_ARG}")
endif()

set(CMAKE_ARGS
  -DBUILD_BENCHMARKS=ON
  -DBUILD_TESTING=ON
  -DBUILD_GOOGLETEST=ON
  -DBUILD_GOOGLEBENCH=ON
  -DBUILD_UMESIMD=${UNIX}
  -DBUILD_VC=${UNIX}
  -DCMAKE_DISABLE_FIND_PACKAGE_PkgConfig=${WIN32}
  -DTARGET_ISA=${CTEST_SCRIPT_ARG}
  $ENV{CMAKE_ARGS}
  ${CMAKE_ARGS}
)

ctest_start(Continuous)
ctest_configure(OPTIONS "${CMAKE_ARGS}")
ctest_build()
ctest_test()
ctest_memcheck()
ctest_submit()
