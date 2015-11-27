# VecCore Tests

add_compile_options(${CMAKE_CXX_FLAGS_TARGET_ISA})

if (SIMD)
    link_libraries(Vc)
endif()

add_executable(backend backend.cc)
