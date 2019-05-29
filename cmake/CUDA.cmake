message(STATUS "Compiling with CUDA enabled")

find_package(CUDA 8.0 REQUIRED)

if(CMAKE_CXX_STANDARD VERSION_GREATER_EQUAL 17)
  message(FATAL_ERROR "CUDA compilation supports only up to ISO C++14 standard")
endif()
