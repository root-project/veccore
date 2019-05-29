cmake_minimum_required(VERSION 3.9)

include(cmake/RecordCmdLine.cmake)

project(VecCore LANGUAGES C CXX VERSION 0.5.2)

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

add_library(VecCore INTERFACE)

include(Builtins)

if (VC)
  find_package(Vc 1.2.0 REQUIRED)

  # Vc does not come with an imported target, add our own
  if(NOT TARGET Vc::Vc)
    add_library(Vc::Vc INTERFACE IMPORTED)
    set_property(TARGET Vc::Vc PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Vc_INCLUDE_DIRS})
    set_property(TARGET Vc::Vc PROPERTY INTERFACE_LINK_LIBRARIES ${Vc_LIBRARIES})
  endif()

  target_compile_definitions(VecCore INTERFACE VECCORE_ENABLE_VC)
  target_link_libraries(VecCore INTERFACE Vc::Vc)
endif()

if (UMESIMD)
  find_package(UMESIMD 0.8.1 REQUIRED)
  target_compile_definitions(VecCore INTERFACE VECCORE_ENABLE_UMESIMD)
  target_link_libraries(VecCore INTERFACE UMESIMD::UMESIMD)
endif()

target_include_directories(VecCore INTERFACE
  $<BUILD_INTERFACE:include ${PROJECT_BINARY_DIR}/include>)

configure_file(include/Config.in include/Config.h)

set(VecCore_INSTALL_INCLUDEDIR "include")
set(VecCore_INSTALL_CMAKEDIR "share/${PROJECT_NAME}/cmake")

include(CMakePackageConfigHelpers)
configure_package_config_file(cmake/VecCoreConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/VecCoreConfig.cmake
  INSTALL_DESTINATION "${VecCore_INSTALL_CMAKEDIR}"
  PATH_VARS VecCore_INSTALL_INCLUDEDIR
  )
configure_file(cmake/VecCoreConfigVersion.cmake.in cmake/VecCoreConfigVersion.cmake @ONLY)

install(FILES "${PROJECT_BINARY_DIR}/include/Config.h" DESTINATION ${VecCore_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
install(DIRECTORY include/${PROJECT_NAME} DESTINATION ${VecCore_INSTALL_INCLUDEDIR})
install(FILES cmake/FindUMESIMD.cmake DESTINATION ${VecCore_INSTALL_CMAKEDIR})
install(DIRECTORY ${PROJECT_BINARY_DIR}/cmake/
  DESTINATION ${VecCore_INSTALL_CMAKEDIR} FILES_MATCHING PATTERN *.cmake)

enable_testing()

if (BUILD_TESTING)
  add_subdirectory(test)
endif()

if (BUILD_BENCHMARKS)
  add_subdirectory(bench)
endif()
