option(DETAILED_TEST_REPORT "Report fine grained test results" OFF)

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

if(DETAILED_TEST_REPORT)
  gtest_discover_tests(test-backends NO_PRETTY_TYPES)
else()
  add_test(NAME test-backends
    COMMAND test-backends --gtest_brief=1)
endif()
