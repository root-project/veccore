message(STATUS "Compiling with CUDA enabled")

find_package(CUDA REQUIRED)

add_definitions(-DVECCORE_ENABLE_CUDA)

if(CMAKE_CXX_STANDARD VERSION_LESS 11)
  message(FATAL_ERROR "CUDA compilation supports only ISO C++ 2011 standard")
endif()

list(APPEND CUDA_NVCC_FLAGS -std=c++${CMAKE_CXX_STANDARD})

set(CUDA_NVCC_FLAGS_DEBUG          "-O2 -g -G")
set(CUDA_NVCC_FLAGS_RELEASE        "-O3 -use_fast_math")
set(CUDA_NVCC_FLAGS_RELWITHDEBINFO "-O3 -g -G -use_fast_math")

set(CUDA_ARCH 30 CACHE STRING "CUDA device architecture")
list(APPEND CUDA_NVCC_FLAGS "-arch=sm_${CUDA_ARCH}")

# Force inlining of memcpy(), workaround for issue below
# https://github.com/tensorflow/tensorflow/issues/1346
list(APPEND CUDA_NVCC_FLAGS "-D_FORCE_INLINES")
