# VecCore Tests

if(USE_EXTERNAL_GTEST)
  find_package(PkgConfig)
  pkg_check_modules(GTEST QUIET gtest)
  if(NOT GTEST_FOUND)
    message(FATAL_ERROR "gtest not found but required when USE_EXTERNAL_GTEST is enabled.")
  endif()
else(USE_EXTERNAL_GTEST)
  add_subdirectory(gtest EXCLUDE_FROM_ALL)
endif()

if (CUDA)
  add_subdirectory(cuda)
endif()

foreach(target align backend math limits traits)
  set(src ${target}.cc)
  add_executable(${target} ${src})
  target_link_libraries(${target} gtest VecCore)
  add_test(${target} ${target})
endforeach()
