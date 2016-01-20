find_package(CUDA REQUIRED)

set(VECCORE_ENABLE_CUDA)

set(CUDA_ARCH 20 CACHE STRING "CUDA device architecture.")
set(CUDA_ARCH "-arch=sm_${CUDA_ARCH}")

set(CMAKE_CUDA_FLAGS_DEBUG          "-O2 -g -G")
set(CMAKE_CUDA_FLAGS_RELEASE        "-O3 -use_fast_math")
set(CMAKE_CUDA_FLAGS_RELWITHDEBINFO "-O3 -g -G -use_fast_math")

set(CMAKE_CUDA_FLAGS ${CMAKE_CUDA_FLAGS} -Wall)
set(CMAKE_CUDA_FLAGS ${CMAKE_CUDA_FLAGS} -Xcudafe "--diag_suppress=code_is_unreachable")
set(CMAKE_CUDA_FLAGS ${CMAKE_CUDA_FLAGS} -Xcudafe "--diag_suppress=initialization_not_reachable")
set(CMAKE_CUDA_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CUDA_FLAGS}")

message(STATUS "Compiling with CUDA enabled")
