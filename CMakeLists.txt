################################################################################
# VecCore Library                                                              #
################################################################################

cmake_minimum_required(VERSION 3.1.0)

project(VecCore VERSION 0.1.0)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 11 CACHE STRING "C++ ISO Standard")
set(CMAKE_CXX_STANDARD_REQUIRED True)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Specifies the build type." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release RelWithDebInfo)
endif()

option(CUDA    "Enable support for CUDA")
option(UMESIMD "Enable UME::SIMD backend")
option(VC      "Enable Vc backend")
option(BENCHMARKS "Enable performance benchmarking" ON)

include(CompilerSetup)

if (VC)
  find_package(Vc 1.2.0 REQUIRED)
  set(VECCORE_ENABLE_VC True)
  include_directories(${Vc_INCLUDE_DIR})
endif()

if (UMESIMD)
  set(VECCORE_ENABLE_UMESIMD True)
  find_path(UMESIMD_INCLUDE_DIR umesimd/UMESimd.h HINTS ${UMESIMD_DIR} $ENV{UMESIMD_DIR})
  if ("${UMESIMD_INCLUDE_DIR}" MATCHES "NOTFOUND")
    message(FATAL_ERROR "UME::SIMD library headers not found")
  else()
    include_directories(${UMESIMD_INCLUDE_DIR})
  endif()
endif()

configure_file(${PROJECT_SOURCE_DIR}/include/Config.in
              "${PROJECT_BINARY_DIR}/include/Config.h")
configure_file(${PROJECT_SOURCE_DIR}/cmake/VecCoreConfig.cmake.in
               ${PROJECT_BINARY_DIR}/cmake/VecCoreConfig.cmake)
configure_file(${PROJECT_SOURCE_DIR}/cmake/VecCoreConfigVersion.cmake.in
               ${PROJECT_BINARY_DIR}/cmake/VecCoreConfigVersion.cmake)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_BINARY_DIR}/include)

install(FILES "${PROJECT_BINARY_DIR}/include/Config.h" DESTINATION include/${PROJECT_NAME})
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME} DESTINATION include)
install(DIRECTORY "${PROJECT_SOURCE_DIR}/cmake/"
        DESTINATION lib${LIB_SUFFIX}/cmake/${PROJECT_NAME} FILES_MATCHING PATTERN *.cmake)
install(DIRECTORY "${PROJECT_BINARY_DIR}/cmake/"
        DESTINATION lib${LIB_SUFFIX}/cmake/${PROJECT_NAME} FILES_MATCHING PATTERN *.cmake)

if (BUILD_TESTING)
  add_subdirectory(test)
  add_subdirectory(bench)
endif()
