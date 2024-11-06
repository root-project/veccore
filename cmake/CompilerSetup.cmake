if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple|)Clang|GNU|Intel|MSVC|NVHPC")
  include(${CMAKE_CXX_COMPILER_ID})
else()
  message(WARNING "Unsupported compiler: ${CMAKE_CXX_COMPILER_ID}")
endif()

if (DEFINED TARGET_ISA AND NOT "${TARGET_ISA}" STREQUAL "")
  string(TOUPPER "${TARGET_ISA}" TARGET_ISA)

  if (NOT "${TARGET_ISA}" MATCHES "^(NATIVE|SSE(2|3|4.(1|2))|AVX(|2|512)|KNC|KNL)$")
    message(FATAL_ERROR "Unknown instruction set architecture: ${TARGET_ISA}")
  endif()

  string(REPLACE "." "" ISA "${TARGET_ISA}")

  if (NOT DEFINED FLAGS_${ISA})
    set(COMPILER "${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
    message(FATAL_ERROR "${TARGET_ISA} not supported by ${COMPILER} compiler")
  else()
    add_compile_options(${FLAGS_${ISA}})
  endif()

  if (NOT DEFINED TARGET_ISA_FLAGS_SET)
    set(TARGET_ISA_FLAGS_SET True CACHE BOOL "")
    message(STATUS "Compiling for ${TARGET_ISA} instruction set architecture")
  endif()
endif()
