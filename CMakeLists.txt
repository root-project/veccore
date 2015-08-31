################################################################################
# VecCore Library                                                              #
################################################################################

project(VecCore)

cmake_minimum_required(VERSION 3.0.2)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake/modules)

include(RequireCXX11)

set(VECCORE_VERSION_MAJOR 0)
set(VECCORE_VERSION_MINOR 1)
set(VECCORE_VERSION_PATCH 0)

set(VECCORE_VERSION ${VECCORE_VERSION_MAJOR}.${VECCORE_VERSION_MINOR}.${VECCORE_VERSION_PATCH})

option(CUDA "Enable support for CUDA.")
option(EXAMPLES "Enable installation of code examples.")
option(MIC "Enable support for Intel Xeon Phi coprocessor.")
option(SIMD "Enable support for SIMD instructions (using Vc library)." ON)
option(TEST "Build VecCore unit tests and benchmarks.")

include(CMakeDependentOption)
cmake_dependent_option(OFFLOAD "Enable Xeon Phi offload support." ON "MIC" OFF)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Specifies the build type." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug;Release;RelWithDebInfo")
endif()

set(TARGET_ISA "native" CACHE STRING "Selects target instruction set architecture to support.")
set_property(CACHE TARGET_ISA PROPERTY STRINGS native sse sse2 sse3 ssse3 sse41 sse42 avx avx2 avx512)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    include (gcc)
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    include (clang)
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "Intel")
    include (icc)
else()
    message(WARNING "Unsupported compiler")
endif()

if (CUDA)
    find_package(CUDA REQUIRED)
    set(VECCORE_ENABLE_CUDA)
endif()

if (SIMD)
    find_package(Vc REQUIRED)
else()
    set(VECCORE_DISABLE_SIMD)
endif()

if (MIC)
    set(VECCORE_ENABLE_MIC)
    if (OFFLOAD)
        set(VECCORE_ENABLE_OFFLOAD)
    endif()
endif()

include_directories(${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME})

configure_file(${PROJECT_SOURCE_DIR}/include/VecCore/VecCore.in
              "${PROJECT_BINARY_DIR}/include/VecCore/VecCore")

if (EXAMPLES)
    add_subdirectory(examples)
endif()

if (TEST)
    add_subdirectory(test)
endif()

install(DIRECTORY include/ DESTINATION include)
