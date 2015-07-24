################################################################################
# VecCore Library                                                              #
################################################################################

cmake_minimum_required(VERSION 3.0)

project(VecCore)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake/modules)

include(RequireCXX11)

option(TEST "Enable testing" OFF)

option(VC "Enable vectorization support with Vc library" ON)

if (VC)
	find_package(Vc REQUIRED)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

if (TEST)
	add_subdirectory(test)
endif()

install(DIRECTORY include/VecCore DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
