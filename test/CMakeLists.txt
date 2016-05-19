# VecCore Tests

add_compile_options(${CMAKE_CXX_FLAGS_TARGET_ISA})

if (VC)
  link_libraries(${Vc_LIBRARIES})
endif()

add_subdirectory(gtest)

add_executable(TestBackend backend.cc)
add_executable(TestMath mathtest.cc)
add_executable(TestNumericLimits limits.cc)
add_executable(TestWrapper wrapper.cc)
add_executable(TestTypeTraits traittest.cc)

target_link_libraries(TestBackend gtest)
target_link_libraries(TestMath gtest)
target_link_libraries(TestTypeTraits gtest)


add_test(Backend TestBackend)
add_test(Traits TestTypeTraits)
add_test(Math TestMath)
add_test(NumericLimits TestNumericLimits)
