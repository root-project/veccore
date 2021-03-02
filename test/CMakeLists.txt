# VecCore Tests

find_package(GTest REQUIRED)

if (CUDA)
  add_subdirectory(cuda)
endif()

foreach(target align backend math limits traits)
  set(src ${target}.cc)
  add_executable(${target} ${src})
  target_link_libraries(${target} GTest::GTest VecCore)
  add_test(${target} ${target})
endforeach()
