################################################################################
# VecCore Library                                                              #
################################################################################

cmake_minimum_required(VERSION 3.1.0)

project(VecCore VERSION 0.1.0)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 11 CACHE STRING "C++ ISO Standard")
set(CMAKE_CXX_STANDARD_REQUIRED True)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

option(CUDA "Enable support for CUDA.")
option(EXAMPLES "Enable installation of code examples.")
option(MIC "Enable support for Intel Xeon Phi coprocessor.")
option(SIMD "Enable support for SIMD instructions (using Vc library).")
option(ENABLE_TESTS "Build VecCore unit tests and benchmarks." ON)

include(CMakeDependentOption)
cmake_dependent_option(OFFLOAD "Enable Xeon Phi offload support." ON "MIC" OFF)

cmake_dependent_option(VC "Enable Vc backend." ON "SIMD" ON)
cmake_dependent_option(UMESIMD "Enable UME::SIMD backend." ON "SIMD" ON)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Specifies the build type." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release RelWithDebInfo)
endif()

include(CompilerSetup)

if (NOT SIMD)
  set(VECCORE_DISABLE_SIMD True)
else()
  if (VC)
    find_package(Vc 1.2.0 REQUIRED)
    set(VECCORE_ENABLE_VC True)
    include_directories(${Vc_INCLUDE_DIR})
  endif()
  if (UMESIMD)
    # UME::SIMD is configured by hand for now
    # find_package(UMESIMD REQUIRED)
    set(VECCORE_ENABLE_UMESIMD True)
    include_directories(${UMESIMD_INCLUDE_DIR})
  endif()
endif()

configure_file(${PROJECT_SOURCE_DIR}/include/Config.in "${PROJECT_BINARY_DIR}/include/Config.h")

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_BINARY_DIR}/include)

install(FILES "${PROJECT_BINARY_DIR}/include/Config.h" DESTINATION include/${PROJECT_NAME})
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME} DESTINATION include)

if (EXAMPLES)
  add_subdirectory(examples)
endif()

if (ENABLE_TESTS)
  enable_testing()
  add_subdirectory(test)
endif()
