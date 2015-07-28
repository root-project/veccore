################################################################################
# VecCore Library                                                              #
################################################################################

cmake_minimum_required(VERSION 3.0)

project(VecCore)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake/modules)

include(RequireCXX11)

option(TEST "Enable testing" OFF)

option(SIMD "Enable SIMD vectorization (using Vc library)" ON)

if (SIMD)
	find_package(Vc REQUIRED)
	set(${VECCORE_CXX_FLAGS} "${VECCORE_CXX_FLAGS} -march=native")
else()
	set(${VECCORE_CXX_FLAGS} "${VECCORE_CXX_FLAGS} -DVECCORE_DISABLE_SIMD")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(${CMAKE_CXX_FLAGS} "${CMAKE_CXX_FLAGS} ${VECCORE_CXX_FLAGS}")

if (TEST)
	add_subdirectory(test)
endif()

install(DIRECTORY include/VecCore DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
