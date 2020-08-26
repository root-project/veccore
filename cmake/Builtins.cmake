include(FetchContent)

macro(clear_var var)
  unset(${var})
  unset(${var} CACHE)
endmacro()

macro(clear_pkg_vars pkg)
  foreach(var FOUND INCLUDE_DIR INCLUDE_DIRS LIBRARY LIBRARIES VERSION)
    clear_var(${pkg}_${var})
  endforeach()
endmacro()

macro(find_package)
  if(NOT "${ARGV0}" IN_LIST BUILTINS)
    _find_package(${ARGV})
  endif()
endmacro()

if(BUILD_VC)
  list(APPEND BUILTINS Vc)
  add_subdirectory(cmake/Vc)
  set(VC ON CACHE BOOL "" FORCE)
else()
  clear_pkg_vars(Vc)
endif()

if(BUILD_UMESIMD)
  list(APPEND BUILTINS UMESIMD)

  FetchContent_Declare(umesimd
    URL "http://lcgpackages.web.cern.ch/lcgpackages/tarFiles/sources/umesimd-0.8.1.tar.gz"
    URL_HASH MD5=3960a11a20a4294e68480ab0d39db6d7
    SOURCE_DIR ${FETCHCONTENT_BASE_DIR}/umesimd)

  FetchContent_GetProperties(umesimd)
  if(NOT umesimd_POPULATED)
    FetchContent_Populate(umesimd)
  endif()

  add_library(UMESIMD::UMESIMD INTERFACE IMPORTED)
  set_target_properties(UMESIMD::UMESIMD PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${FETCHCONTENT_BASE_DIR})

  # Awkward manual install due to UMESIMD's bad packaging
  install(DIRECTORY ${umesimd_SOURCE_DIR}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/umesimd
    PATTERN "CMakeLists.txt" EXCLUDE
    PATTERN "README.md" EXCLUDE
    PATTERN "doc" EXCLUDE
    PATTERN "examples" EXCLUDE
    PATTERN "microbenchmarks" EXCLUDE
    PATTERN "unittest" EXCLUDE)

  set(UMESIMD ON CACHE BOOL "" FORCE)
else()
  clear_pkg_vars(UMESIMD)
endif()
