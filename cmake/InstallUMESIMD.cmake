include(ExternalProject)

set(UMESIMD_VERSION "0.8.1")
set(UMESIMD_PROJECT "umesimd-${UMESIMD_VERSION}")
set(UMESIMD_SRC_MD5 "3960a11a20a4294e68480ab0d39db6d7")
set(UMESIMD_ROOTDIR "${CMAKE_BINARY_DIR}/${UMESIMD_PROJECT}")
set(UMESIMD_DESTDIR "${CMAKE_BINARY_DIR}/${UMESIMD_PROJECT}/include/umesimd")
set(UMESIMD_SOURCES "${CMAKE_BINARY_DIR}/externals/src/${UMESIMD_PROJECT}")

set(UMESIMD_DST_DIR "${CMAKE_BINARY_DIR}/${UMESIMD_PROJECT}/include/umesimd")

ExternalProject_Add(${UMESIMD_PROJECT}
  PREFIX externals
  URL "http://lcgpackages.web.cern.ch/lcgpackages/tarFiles/sources/${UMESIMD_PROJECT}.tar.gz"
  URL_MD5 ${UMESIMD_SRC_MD5}
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND
    ${CMAKE_COMMAND} -E copy_directory ${UMESIMD_SOURCES}/ ${UMESIMD_DESTDIR}
    && ${CMAKE_COMMAND} -E remove ${UMESIMD_DESTDIR}/CMakeLists.txt
    && ${CMAKE_COMMAND} -E remove ${UMESIMD_DESTDIR}/README.md
    && ${CMAKE_COMMAND} -E remove_directory ${UMESIMD_DESTDIR}/doc
    && ${CMAKE_COMMAND} -E remove_directory ${UMESIMD_DESTDIR}/examples
    && ${CMAKE_COMMAND} -E remove_directory ${UMESIMD_DESTDIR}/microbenchmarks
    && ${CMAKE_COMMAND} -E remove_directory ${UMESIMD_DESTDIR}/unittest
)

add_custom_target(UMESIMD)
add_dependencies(UMESIMD ${UMESIMD_PROJECT})

set(UMESIMD_INCLUDE_DIRS ${UMESIMD_ROOTDIR}/include)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(UMESIMD
  REQUIRED_VARS UMESIMD_INCLUDE_DIRS VERSION_VAR UMESIMD_VERSION)

install(DIRECTORY ${UMESIMD_ROOTDIR}/ DESTINATION ${CMAKE_INSTALL_PREFIX})
