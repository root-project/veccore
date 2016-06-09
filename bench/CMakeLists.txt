# VecCore Benchmarks

if (VC)
  link_libraries(${Vc_LIBRARIES})
endif()

add_executable(quadratic quadratic.cc)
add_test(QuadraticBenchmark quadratic)
