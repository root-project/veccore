include(ExternalProject)

set(UMESIMD_VERSION "0.8.1")
set(UMESIMD_PROJECT "umesimd-${UMESIMD_VERSION}")
set(UMESIMD_SRC_URI "https://github.com/edanor/umesimd")
set(UMESIMD_SRC_MD5 "3960a11a20a4294e68480ab0d39db6d7")
set(UMESIMD_SRC_DIR "${CMAKE_BINARY_DIR}/externals/src/${UMESIMD_PROJECT}")
set(UMESIMD_DST_DIR "${CMAKE_BINARY_DIR}/${UMESIMD_PROJECT}/include/umesimd")

ExternalProject_Add(${UMESIMD_PROJECT}
  PREFIX externals
  URL "${UMESIMD_SRC_URI}/archive/v${UMESIMD_VERSION}.tar.gz"
  URL_MD5 ${UMESIMD_SRC_MD5}
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND
    ${CMAKE_COMMAND} -E copy_directory ${UMESIMD_SRC_DIR}/ ${UMESIMD_DST_DIR}
    && ${CMAKE_COMMAND} -E remove ${UMESIMD_DST_DIR}/CMakeLists.txt
    && ${CMAKE_COMMAND} -E remove ${UMESIMD_DST_DIR}/README.md
    && ${CMAKE_COMMAND} -E remove_directory ${UMESIMD_DST_DIR}/doc
    && ${CMAKE_COMMAND} -E remove_directory ${UMESIMD_DST_DIR}/examples
    && ${CMAKE_COMMAND} -E remove_directory ${UMESIMD_DST_DIR}/microbenchmarks
    && ${CMAKE_COMMAND} -E remove_directory ${UMESIMD_DST_DIR}/unittest
)

add_custom_target(UMESIMD)
add_dependencies(UMESIMD ${UMESIMD_PROJECT})

set(UMESIMD_INCLUDE_DIRS ${UMESIMD_DESTDIR}/include CACHE INTERNAL "UMESIMD include directories")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(UMESIMD
  REQUIRED_VARS UMESIMD_INCLUDE_DIRS VERSION_VAR UMESIMD_VERSION)

install(DIRECTORY ${CMAKE_BINARY_DIR}/${UMESIMD_PROJECT}/ DESTINATION ${CMAKE_INSTALL_PREFIX})
