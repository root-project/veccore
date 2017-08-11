# VecCore Tests

add_subdirectory(gtest EXCLUDE_FROM_ALL)

if (CUDA)
  add_subdirectory(cuda)
endif()

add_executable(Alignment align.cc)
add_executable(Backend backend.cc)
add_executable(TypeTraits traittest.cc)
add_executable(NumericLimits limits.cc)
add_executable(Math mathtest.cc)

if (DEFINED VecCore_LIBRARIES)
  target_link_libraries(Alignment ${VecCore_LIBRARIES})
  target_link_libraries(Backend ${VecCore_LIBRARIES})
  target_link_libraries(TypeTraits ${VecCore_LIBRARIES})
  target_link_libraries(NumericLimits ${VecCore_LIBRARIES})
  target_link_libraries(Math ${VecCore_LIBRARIES})
endif()

if (VECCORE_ENABLE_UMESIMD)
  add_dependencies(Alignment UMESIMD)
  add_dependencies(Backend UMESIMD)
  add_dependencies(TypeTraits UMESIMD)
  add_dependencies(NumericLimits UMESIMD)
  add_dependencies(Math UMESIMD)
endif()

target_link_libraries(Alignment gtest)
target_link_libraries(Backend gtest)
target_link_libraries(TypeTraits gtest)
target_link_libraries(Math gtest)

add_test(Alignment Alignment)
add_test(Backend Backend)
add_test(TypeTraits TypeTraits)
add_test(NumericLimits NumericLimits)
add_test(Math Math)
