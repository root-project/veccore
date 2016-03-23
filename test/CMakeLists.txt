# VecCore Tests

add_compile_options(${CMAKE_CXX_FLAGS_TARGET_ISA})

if (SIMD)
  link_libraries(${Vc_LIBRARIES})
endif()

add_executable(backend backend.cc)
add_executable(limits limits.cc)
add_executable(wrapper wrapper.cc)

add_test(Backend backend)
add_test(NumericLimits limits)
