set(UMESIMD_VERSION "0.6.1")
set(UMESIMD_PROJECT "umesimd-${UMESIMD_VERSION}")
set(UMESIMD_SRC_URI "https://github.com/edanor/umesimd")
set(UMESIMD_DESTDIR "${CMAKE_BINARY_DIR}/${UMESIMD_PROJECT}")

ExternalProject_Add(${UMESIMD_PROJECT}
  PREFIX ext
  GIT_REPOSITORY "${UMESIMD_SRC_URI}"
  GIT_TAG "v${UMESIMD_VERSION}-stable"
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND
  install -d ${UMESIMD_DESTDIR}/include/umesimd &&
  cd ${UMESIMD_DESTDIR}/include/umesimd &&
  cp -nRT ${CMAKE_BINARY_DIR}/ext/src/${UMESIMD_PROJECT} . &&
  rm -rf .git doc examples microbenchmarks unittest README.md
)

add_custom_target(UMESIMD)
add_dependencies(UMESIMD ${UMESIMD_PROJECT})

set(UMESIMD_INCLUDE_DIRS ${UMESIMD_DESTDIR}/include CACHE INTERNAL "UMESIMD include directories")

install(DIRECTORY ${UMESIMD_DESTDIR}/ DESTINATION ${CMAKE_INSTALL_PREFIX})
