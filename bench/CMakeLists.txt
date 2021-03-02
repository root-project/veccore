# VecCore Benchmarks

add_subdirectory(gbench EXCLUDE_FROM_ALL)

# Avoid deprecation warning from Vc
if(NOT MSVC)
  add_compile_options(-Wno-deprecated-declarations)
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  # prevent ICC from issuing streaming stores,
  # which obfuscate the timings for benchmarks
  add_compile_options(-qopt-streaming-stores=never)
endif()

add_executable(mathbench math_benchmarks.cc)
target_link_libraries(mathbench benchmark benchmark_main VecCore)

add_executable(mathbench-fast math_benchmarks.cc)
target_compile_options(mathbench-fast PRIVATE
  $<IF:$<BOOL:${MSVC}>,/fp:fast,-ffast-math>)
target_link_libraries(mathbench-fast benchmark benchmark_main VecCore)

add_executable(quadratic quadratic.cc)
target_link_libraries(quadratic VecCore)

add_executable(nbody nbody.cc)
target_link_libraries(nbody VecCore)

find_package(PkgConfig)

if(PKG_CONFIG_FOUND)
  pkg_check_modules(GD IMPORTED_TARGET gdlib)
endif()

add_library(png STATIC png.cc)

if (GD_FOUND)
  target_compile_definitions(png PUBLIC HAVE_GD)
  target_link_libraries(png PUBLIC PkgConfig::GD)
endif()

foreach(target julia mandelbrot newton)
  add_executable(${target} ${target}.cc)
  target_link_libraries(${target} png VecCore)
endforeach()

if(BUILD_TESTING)
  foreach(target julia mandelbrot newton quadratic nbody)
    add_test(NAME bench-${target} COMMAND ${target})
  endforeach()
endif()
