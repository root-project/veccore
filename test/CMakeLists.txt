# VecCore Tests

add_subdirectory(gtest)

if (CUDA)
  add_subdirectory(cuda)
endif()

if (VC)
  link_libraries(${Vc_LIBRARIES})
endif()

add_executable(Backend backend.cc)
add_executable(TypeTraits traittest.cc)
add_executable(NumericLimits limits.cc)
add_executable(Math mathtest.cc)

target_link_libraries(Backend gtest)
target_link_libraries(TypeTraits gtest)
target_link_libraries(Math gtest)

add_test(Backend Backend)
add_test(TypeTraits TypeTraits)
add_test(NumericLimits NumericLimits)
add_test(Math Math)
