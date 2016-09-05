# VecCore Benchmarks

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  # prevent ICC from issuing streaming stores,
  # which obfuscate the timings for benchmarks
  add_compile_options(-qopt-streaming-stores=never)
endif()

if (VC)
  link_libraries(${Vc_LIBRARIES})
endif()

add_executable(quadratic quadratic.cc)
add_test(QuadraticBenchmark quadratic)
