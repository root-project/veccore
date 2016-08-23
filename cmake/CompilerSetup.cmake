set(TARGET_ISA "native" CACHE STRING "Select target instruction set architecture")

string(TOUPPER "${TARGET_ISA}" TARGET_ISA)
if (NOT "${TARGET_ISA}" MATCHES "^(NATIVE|SSE(2|3|4.(1|2))|AVX(|2|512)|KNC|KNL)$")
  message(FATAL_ERROR "Unknown target instruction set architecture: ${TARGET_ISA}")
endif()

add_compile_options(-Wall)

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple|)Clang|GNU|Intel")
  include(${CMAKE_CXX_COMPILER_ID})
else()
  message(WARNING "Unsupported compiler: ${CMAKE_CXX_COMPILER_ID}")
endif()

string(REPLACE "." "" ISA "${TARGET_ISA}")

if(NOT DEFINED FLAGS_${ISA})
  set(COMPILER "${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
  message(FATAL_ERROR "${TARGET_ISA} not supported with ${COMPILER} compiler")
endif()

message(STATUS "Compiling for ${TARGET_ISA} SIMD instructions")

add_compile_options(${FLAGS_${ISA}})
