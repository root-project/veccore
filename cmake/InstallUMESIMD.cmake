set(UMESIMD_VERSION "0.6.1")
set(UMESIMD_PROJECT "umesimd-${UMESIMD_VERSION}")
set(UMESIMD_SRC_URI "https://github.com/edanor/umesimd")

ExternalProject_Add(${UMESIMD_PROJECT}
  PREFIX ext
  GIT_REPOSITORY "${UMESIMD_SRC_URI}"
  GIT_TAG "v${UMESIMD_VERSION}-stable"
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND
  install -d <INSTALL_DIR>/install/include/umesimd &&
  cd <INSTALL_DIR>/install/include/umesimd &&
  cp -nRT ${CMAKE_BINARY_DIR}/ext/src/${UMESIMD_PROJECT} . &&
  rm -rf .git doc examples microbenchmarks unittest README.md
)

add_custom_target(UMESIMD)
add_dependencies(UMESIMD ${UMESIMD_PROJECT})

ExternalProject_Get_Property(${UMESIMD_PROJECT} install_dir)

set(UMESIMD_ROOT ${install_dir} CACHE INTERNAL "UMESIMD ROOT directory")
set(UMESIMD_LIBRARIES)
set(UMESIMD_LIBRARIES_OPTIONAL)
set(UMESIMD_INCLUDE_DIRS ${install_dir}/include CACHE INTERNAL "UMESIMD include directories")
set(UMESIMD_DEFINITIONS "" CACHE INTERNAL "UMESIMD definitions")
