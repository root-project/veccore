
set(TARGET_ISA "native" CACHE STRING "Selects target instruction set architecture to support.")
set_property(CACHE TARGET_ISA PROPERTY STRINGS native sse sse2 sse3 ssse3 sse41 sse42 avx avx2 avx512 mic)

message(STATUS "Compiling for ${TARGET_ISA} SIMD instructions")

add_compile_options(-Wall)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  include (GCC)
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" OR
        ${CMAKE_CXX_COMPILER_ID} STREQUAL "AppleClang")
  include (Clang)
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "Intel")
  include (Intel)
else()
  message(WARNING "Unsupported compiler")
endif()

string(TOUPPER "FLAGS_${TARGET_ISA}" _target_isa)
set(CMAKE_CXX_FLAGS_TARGET_ISA "${${_target_isa}}")

if (CUDA)
  include(CUDA)
endif()
