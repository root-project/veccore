cmake_minimum_required(VERSION 3.9)

include(cmake/RecordCmdLine.cmake)

project(VecCore LANGUAGES C CXX VERSION 0.7.0
    DESCRIPTION "C++ Library for Portable SIMD Vectorization")

set(CMAKE_CXX_STANDARD 11 CACHE STRING "C++ ISO Standard")
set(CMAKE_CXX_STANDARD_REQUIRED True)

list(INSERT CMAKE_MODULE_PATH 0 ${PROJECT_SOURCE_DIR}/cmake)

option(CUDA    "Enable support for CUDA")
option(UMESIMD "Enable UME::SIMD backend")
option(VC      "Enable Vc backend")

option(BUILD_DOCS         "Build doxygen documentation")
option(BUILD_BENCHMARKS   "Build binaries for performance benchmarking")
option(BUILD_TESTING      "Build test binaries and create test target")
option(USE_EXTERNAL_GTEST "Use external gtest" ON)

option(BUILD_UMESIMD  "Build UME::SIMD library from source")
option(BUILD_VC       "Build Vc library from source")

include(CompilerSetup)

if (CUDA)
  include(CUDA)
endif()

add_library(VecCore INTERFACE)

include(Builtins)

if (VC)
  find_package(Vc 1.3.3 REQUIRED)

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

if (CUDA)
  target_compile_definitions(VecCore INTERFACE VECCORE_ENABLE_CUDA)
  target_include_directories(VecCore INTERFACE ${CUDA_INCLUDE_DIRS})
  target_link_libraries(VecCore INTERFACE ${CUDA_LIBRARIES})
endif()

include_directories(include)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

write_basic_package_version_file(cmake/VecCoreConfigVersion.cmake
  VERSION ${VecCore_VERSION} COMPATIBILITY SameMajorVersion)

configure_package_config_file(cmake/VecCoreConfig.cmake.in cmake/VecCoreConfig.cmake
  INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
  PATH_VARS CMAKE_INSTALL_INCLUDEDIR
)

install(DIRECTORY include/${PROJECT_NAME}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES cmake/FindUMESIMD.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
install(DIRECTORY ${PROJECT_BINARY_DIR}/cmake/
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

if (BUILD_DOCS)
  add_subdirectory(doc)
endif()

enable_testing()

if (BUILD_TESTING)
  add_subdirectory(test)
endif()

if (BUILD_BENCHMARKS)
  add_subdirectory(bench)
endif()
