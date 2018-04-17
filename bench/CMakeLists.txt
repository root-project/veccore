# VecCore Benchmarks

# Avoid deprecation warning from Vc
add_compile_options(-Wno-deprecated-declarations)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  # prevent ICC from issuing streaming stores,
  # which obfuscate the timings for benchmarks
  add_compile_options(-qopt-streaming-stores=never)
endif()

add_executable(quadratic quadratic.cc)
target_link_libraries(quadratic VecCore)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GD IMPORTED_TARGET gdlib)

add_library(png STATIC png.cc)

if (GD_FOUND)
  target_compile_definitions(png PUBLIC HAVE_GD)
  target_link_libraries(png INTERFACE PkgConfig::GD)
endif()

foreach(target julia mandelbrot newton)
  add_executable(${target} ${target}.cc)
  target_link_libraries(${target} png VecCore)
endforeach()
