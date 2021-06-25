cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

if (CUDA)
  add_subdirectory(cuda)
endif()

find_package(GTest REQUIRED)

include(GoogleTest)

add_executable(test-backends
  align.cc
  arithmetics.cc
  constructors.cc
  interface.cc
  limits.cc
  mask.cc
  math.cc
  traits.cc
)

target_link_libraries(test-backends
  GTest::GTest GTest::Main VecCore)

gtest_discover_tests(test-backends NO_PRETTY_TYPES)
