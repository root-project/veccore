# VecCore Benchmarks

# Avoid deprecation warning from Vc
add_compile_options(-Wno-deprecated-declarations)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  # prevent ICC from issuing streaming stores,
  # which obfuscate the timings for benchmarks
  add_compile_options(-qopt-streaming-stores=never)
endif()

add_executable(mandelbrot mandelbrot.cc png.cc)
add_executable(julia julia.cc png.cc)
add_executable(newton newton.cc png.cc)
add_executable(quadratic quadratic.cc)

if(DEFINED VecCore_LIBRARIES)
  target_link_libraries(mandelbrot ${VecCore_LIBRARIES})
  target_link_libraries(julia ${VecCore_LIBRARIES})
  target_link_libraries(newton ${VecCore_LIBRARIES})
  target_link_libraries(quadratic ${VecCore_LIBRARIES})
endif()

if(VECCORE_ENABLE_UMESIMD)
  add_dependencies(mandelbrot UMESIMD)
  add_dependencies(julia UMESIMD)
  add_dependencies(newton UMESIMD)
  add_dependencies(quadratic UMESIMD)
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(GD IMPORTED_TARGET gdlib)

if (GD_FOUND)
  target_compile_definitions(mandelbrot PRIVATE -DHAVE_GD)
  target_compile_definitions(julia PRIVATE -DHAVE_GD)
  target_compile_definitions(newton PRIVATE -DHAVE_GD)
  target_link_libraries(mandelbrot PkgConfig::GD)
  target_link_libraries(julia PkgConfig::GD)
  target_link_libraries(newton PkgConfig::GD)
endif()
