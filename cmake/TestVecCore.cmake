set(CTEST_PROJECT_NAME "VecCore")

set(CTEST_SOURCE_DIRECTORY "$ENV{PWD}")
set(CTEST_BINARY_DIRECTORY "$ENV{PWD}/build")

set(CTEST_CMAKE_GENERATOR "Unix Makefiles")

if(DEFINED ENV{TARGET_ISA})
  set(TARGET_ISA $ENV{TARGET_ISA})
else()
  set(TARGET_ISA SSE4.2)
endif()

set(options
  -DBUILD_BENCHMARKS=ON
  -DBUILD_TESTING=ON
  -DBUILD_UMESIMD=ON
  -DBUILD_VC=ON
  -DTARGET_ISA=${TARGET_ISA}
)

ctest_empty_binary_directory(${CTEST_BINARY_DIRECTORY})

ctest_start(Continuous)

ctest_configure(OPTIONS "${options}" RETURN_VALUE status)
if(NOT ${status} EQUAL 0)
  message(FATAL_ERROR "Failed to configure project")
endif()

ctest_build(FLAGS -j)
ctest_test()
