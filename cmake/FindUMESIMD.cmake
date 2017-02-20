#.rst:
# FindUMESIMD
# -----------
#
# Find the UME::SIMD library headers and define variables.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This module defines the following variables:
#
# ::
#
#   UMESIMD_FOUND          - True if UME::SIMD is found.
#   UMESIMD_INCLUDE_DIRS   - Where to find umesimd.h, etc.
#
# ::
#
#   UMESIMD_VERSION        - The version of UME::SIMD found (x.y.z)
#   UMESIMD_VERSION_MAJOR  - The major version of UME::SIMD
#   UMESIMD_VERSION_MINOR  - The minor version of UME::SIMD
#   UMESIMD_VERSION_PATCH  - The patch version of UME::SIMD
#
# Hints
# ^^^^^
#
# A user may set ``UMESIMD_ROOT`` to a UME::SIMD installation root to tell this
# module where to look.

set(_UMESIMD_PATHS)

if(UMESIMD_ROOT)
  set(_UMESIMD_SEARCH_ROOT PATHS ${UMESIMD_ROOT} NO_DEFAULT_PATH)
  list(APPEND _UMESIMD_PATHS _UMESIMD_SEARCH_ROOT)
endif()

if(EXISTS $ENV{UMESIMD_ROOT})
  set(_UMESIMD_SEARCH_ROOT PATHS "$ENV{UMESIMD_ROOT}" NO_DEFAULT_PATH)
  list(APPEND _UMESIMD_PATHS _UMESIMD_SEARCH_ROOT)
endif()

foreach(search ${_UMESIMD_PATHS})
  find_path(UMESIMD_INCLUDE_DIR NAMES umesimd/UMESimd.h ${${search}} PATH_SUFFIXES include)
endforeach()

mark_as_advanced(UMESIMD_INCLUDE_DIR)

if(UMESIMD_INCLUDE_DIR AND EXISTS "${UMESIMD_INCLUDE_DIR}/umesimd/README.md")
    file(STRINGS "${UMESIMD_INCLUDE_DIR}/umesimd/README.md" UMESIMD_VERSION REGEX "^.*Current stable release.*$")
    string(REGEX REPLACE "^[^0-9]*([0-9\.]+).*$" "\\1" UMESIMD_VERSION "${UMESIMD_VERSION}")
    string(REGEX REPLACE "^([0-9]+).*$" "\\1" UMESIMD_VERSION_MAJOR "${UMESIMD_VERSION}")
    string(REGEX REPLACE "^[0-9]+\\.([0-9]+).*$" "\\1" UMESIMD_VERSION_MINOR  "${UMESIMD_VERSION}")
    string(REGEX REPLACE "^[0-9]+\\.[0-9]+\\.([0-9]+).*$" "\\1" UMESIMD_VERSION_PATCH "${UMESIMD_VERSION}")
    set(UMESIMD_VERSION_STRING "${UMESIMD_VERSION}")
endif()

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(UMESIMD REQUIRED_VARS UMESIMD_INCLUDE_DIR VERSION_VAR UMESIMD_VERSION)

if(UMESIMD_FOUND)
    set(UMESIMD_INCLUDE_DIRS ${UMESIMD_INCLUDE_DIR})
endif()
