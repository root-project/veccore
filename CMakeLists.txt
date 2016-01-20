################################################################################
# VecCore Library                                                              #
################################################################################

cmake_minimum_required(VERSION 3.0.2)

project(VecCore VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

option(CUDA "Enable support for CUDA.")
option(EXAMPLES "Enable installation of code examples.")
option(MIC "Enable support for Intel Xeon Phi coprocessor.")
option(SIMD "Enable support for SIMD instructions (using Vc library)." ON)
option(ENABLE_TESTS "Build VecCore unit tests and benchmarks." ON)

include(CMakeDependentOption)
cmake_dependent_option(OFFLOAD "Enable Xeon Phi offload support." ON "MIC" OFF)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Specifies the build type." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release RelWithDebInfo)
endif()

include(CompilerSetup)

if (NOT SIMD)
  set(VECCORE_DISABLE_SIMD True)
else()
  find_package(Vc REQUIRED)
  set(VECCORE_ENABLE_VC True)
endif()

configure_file(${PROJECT_SOURCE_DIR}/include/Config.in
  "${PROJECT_BINARY_DIR}/include/${PROJECT_NAME}/Config.h")
file(COPY include/${PROJECT_NAME} DESTINATION ${PROJECT_BINARY_DIR}/include)

include_directories(${PROJECT_BINARY_DIR}/include/${PROJECT_NAME})
install(DIRECTORY ${PROJECT_BINARY_DIR}/include/ DESTINATION include)

if (EXAMPLES)
  add_subdirectory(examples)
endif()

if (ENABLE_TESTS)
  enable_testing()
  add_subdirectory(test)
endif()
