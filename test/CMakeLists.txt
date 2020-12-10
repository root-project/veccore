# VecCore Tests

add_subdirectory(gtest EXCLUDE_FROM_ALL)

if (CUDA)
  add_subdirectory(cuda)
endif()

foreach(target align backend math limits traits)
  set(src ${target}.cc)
  add_executable(${target} ${src})
  target_link_libraries(${target} gtest gtest_main VecCore)
  add_test(${target} ${target})
endforeach()
