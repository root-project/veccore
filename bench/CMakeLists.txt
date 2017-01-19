# VecCore Benchmarks

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  # prevent ICC from issuing streaming stores,
  # which obfuscate the timings for benchmarks
  add_compile_options(-qopt-streaming-stores=never)
endif()

add_executable(quadratic quadratic.cc)

if(VECCORE_ENABLE_VC)
  target_link_libraries(quadratic ${Vc_LIBRARIES})
endif()

if(VECCORE_ENABLE_UMESIMD)
  add_dependencies(quadratic UMESIMD)
endif()

add_test(QuadraticBenchmark quadratic)
