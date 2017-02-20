# VecCore Tests

add_subdirectory(gtest)

if (CUDA)
  add_subdirectory(cuda)
endif()

add_executable(Alignment align.cc)
add_executable(Backend backend.cc)
add_executable(TypeTraits traittest.cc)
add_executable(NumericLimits limits.cc)
add_executable(Math mathtest.cc)

if (VECCORE_ENABLE_VC)
  target_link_libraries(Alignment ${Vc_LIBRARIES})
  target_link_libraries(Backend ${Vc_LIBRARIES})
  target_link_libraries(TypeTraits ${Vc_LIBRARIES})
  target_link_libraries(NumericLimits ${Vc_LIBRARIES})
  target_link_libraries(Math ${Vc_LIBRARIES})
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
