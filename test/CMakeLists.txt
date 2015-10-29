# VecCore Tests

add_compile_options(${CMAKE_CXX_FLAGS_TARGET_ISA})

if (SIMD)
    link_libraries(Vc)
endif()

add_executable(backend backend.cc)
add_executable(geometry geometry.cc)
add_executable(transform transform.cc)
add_executable(quaternions quaternions.cc)
add_executable(composition composition.cc)
