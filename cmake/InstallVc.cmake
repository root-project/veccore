set(Vc_VERSION "1.3.0")
set(Vc_PROJECT  "Vc-${Vc_VERSION}")
set(Vc_SRC_URI "https://github.com/VcDevel/Vc")
set(Vc_SRC_MD5 "a248e904f0b1a330ad8f37ec50cbad30")

ExternalProject_Add(${Vc_PROJECT}
  PREFIX ext
  URL "${Vc_SRC_URI}/releases/download/${Vc_VERSION}/${Vc_PROJECT}.tar.gz"
  URL_MD5 ${Vc_SRC_MD5}
  BUILD_IN_SOURCE 0
  CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
             -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
             -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS} 
             -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
             -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
             -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>/install
)

ExternalProject_Get_Property(${Vc_PROJECT} INSTALL_DIR)

add_library(Vc UNKNOWN IMPORTED)
set_property(TARGET Vc PROPERTY IMPORTED_LOCATION ${INSTALL_DIR}/install/lib/libVc${CMAKE_STATIC_LIBRARY_SUFFIX})
add_dependencies(Vc ${Vc_PROJECT})

set(Vc_ROOT ${INSTALL_DIR} CACHE INTERNAL "Vc ROOT directory")
set(Vc_LIBRARIES Vc)
set(Vc_LIBRARIES_OPTIONAL ${Vc_LIBRARIES})
set(Vc_INCLUDE_DIRS ${INSTALL_DIR}/install/include CACHE INTERNAL "Vc include directories")
set(Vc_DEFINITIONS "" CACHE INTERNAL "Vc definitions")
