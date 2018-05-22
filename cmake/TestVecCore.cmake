set(CTEST_PROJECT_NAME "VecCore")

set(CTEST_SOURCE_DIRECTORY "$ENV{PWD}")
set(CTEST_BINARY_DIRECTORY "$ENV{PWD}/build")

set(CTEST_CMAKE_GENERATOR "Unix Makefiles")

set(options
  -DBUILD_BENCHMARKS=ON
  -DBUILD_TESTING=ON
  -DBUILD_UMESIMD=ON
  -DBUILD_VC=ON
  -DTARGET_ISA=SSE4.2
  )

ctest_start(Continuous)
ctest_configure(OPTIONS "${options}")
ctest_build()
ctest_test()
