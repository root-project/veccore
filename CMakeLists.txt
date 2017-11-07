################################################################################
# VecCore Library                                                              #
################################################################################

cmake_minimum_required(VERSION 3.6.0)

include(cmake/RecordCmdLine.cmake)

project(VecCore LANGUAGES C CXX VERSION 0.4.2)

set(CMAKE_CXX_STANDARD 11 CACHE STRING "C++ ISO Standard")
set(CMAKE_CXX_STANDARD_REQUIRED True)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Select build type" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release RelWithDebInfo)
endif()

option(CUDA    "Enable support for CUDA")
option(UMESIMD "Enable UME::SIMD backend")
option(VC      "Enable Vc backend")

option(BUILD_BENCHMARKS "Build binaries for performance benchmarking")
option(BUILD_TESTING    "Build test binaries and create test target")

option(BUILD_UMESIMD  "Build UME::SIMD library from source")
option(BUILD_VC       "Build Vc library from source")

include(CompilerSetup)

if (CUDA)
  include(CUDA)
endif()

if (VC OR BUILD_VC)
  set(VC ON CACHE BOOL "Enable Vc backend" FORCE)
  add_definitions(-DVECCORE_ENABLE_VC)
  if (BUILD_VC)
    include(InstallVc)
  else()
    find_package(Vc 1.2.0 REQUIRED)
    if (KNC)
      set(VecCore_LIBRARIES ${VecCore_LIBRARIES} ${Vc_MIC_LIBRARIES})
    else()
      set(VecCore_LIBRARIES ${VecCore_LIBRARIES} ${Vc_LIBRARIES})
    endif()
  endif()
  include_directories(SYSTEM BEFORE ${Vc_INCLUDE_DIRS})
endif()

if (UMESIMD OR BUILD_UMESIMD)
  set(UMESIMD ON CACHE BOOL "Enable UME::SIMD backend" FORCE)
  add_definitions(-DVECCORE_ENABLE_UMESIMD)
  if (BUILD_UMESIMD)
    include(InstallUMESIMD)
  else()
    find_package(UMESIMD 0.8.1 REQUIRED)
  endif()
  include_directories(SYSTEM BEFORE ${UMESIMD_INCLUDE_DIRS})
endif()

include_directories(include ${PROJECT_BINARY_DIR}/include)

configure_file(include/Config.in include/Config.h)
configure_file(cmake/VecCoreConfig.cmake.in cmake/VecCoreConfig.cmake @ONLY)
configure_file(cmake/VecCoreConfigVersion.cmake.in cmake/VecCoreConfigVersion.cmake @ONLY)

install(FILES "${PROJECT_BINARY_DIR}/include/Config.h" DESTINATION include/${PROJECT_NAME})
install(DIRECTORY include/${PROJECT_NAME} DESTINATION include)
install(FILES cmake/FindUMESIMD.cmake DESTINATION lib${LIB_SUFFIX}/cmake/${PROJECT_NAME})
install(DIRECTORY ${PROJECT_BINARY_DIR}/cmake/
        DESTINATION lib${LIB_SUFFIX}/cmake/${PROJECT_NAME} FILES_MATCHING PATTERN *.cmake)

enable_testing()

if (BUILD_TESTING)
  add_subdirectory(test)
endif()

if (BUILD_BENCHMARKS)
  add_subdirectory(bench)
endif()
