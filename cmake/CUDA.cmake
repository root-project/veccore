if(NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  message(FATAL_ERROR "CUDA compilation requires GCC compiler")
else()
  if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 5.0.0
      OR CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 5.0.0)
    message(FATAL_ERROR "GCC ${CMAKE_CXX_COMPILER_VERSION} is not supported by CUDA")
  endif()
endif()

find_package(CUDA REQUIRED)

set(VECCORE_ENABLE_CUDA True)

if(CMAKE_CXX_STANDARD STREQUAL 11)
  list(APPEND CUDA_NVCC_FLAGS -std=c++11)
else()
  message(FATAL_ERROR "CUDA compilation supports only ISO C++ 2011 standard")
endif()

set(CUDA_NVCC_FLAGS_DEBUG          "-O2 -g -G")
set(CUDA_NVCC_FLAGS_RELEASE        "-O3 -use_fast_math")
set(CUDA_NVCC_FLAGS_RELWITHDEBINFO "-O3 -g -G -use_fast_math")

list(APPEND CUDA_NVCC_FLAGS "-gencode arch=compute_30,code=sm_30")

message(STATUS "Compiling with CUDA enabled")
