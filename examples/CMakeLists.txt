# VecCore Examples

if (CUDA)
  add_subdirectory(cuda)
endif()

add_compile_options(${CMAKE_CXX_FLAGS_TARGET_ISA})

if (SIMD)
  link_libraries(${Vc_LIBRARIES})
endif()

add_executable(quadratic quadratic.cc)
