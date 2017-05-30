# Try to capture the initial set of cmake command line args passed by
# the user for configuration. Original Recipe taken from the link below:
# http://stackoverflow.com/questions/10205986/how-to-capture-cmake-command-line-arguments
#
# Note: The entries will live on CMakeCache.txt, so re-configuring with
# a command line that doesn't include an option won't remove it. You need
# to remove the CMakeCache.txt file, or override the value via the command line.
#
# The original cmake command is recorded as:
#     recmake_initial.sh
# The history of tweaks to the configuration (except those from ccmake) is
# recorded as:
#     recmake_replay.sh

get_cmake_property(CACHE_VARS CACHE_VARIABLES)
foreach (CACHE_VAR ${CACHE_VARS})
  get_property(CACHE_VAR_HELPSTRING CACHE ${CACHE_VAR} PROPERTY HELPSTRING)
  if (CACHE_VAR_HELPSTRING STREQUAL "No help, variable specified on the command line.")
    get_property(CACHE_VAR_TYPE CACHE ${CACHE_VAR} PROPERTY TYPE)
    if (CACHE_VAR_TYPE STREQUAL "UNINITIALIZED")
      set(CACHE_VAR_TYPE)
    else()
      set(CACHE_VAR_TYPE :${CACHE_VAR_TYPE})
    endif()
    set(CMAKE_INVOKE_ARGS "${CMAKE_INVOKE_ARGS} -D${CACHE_VAR}${CACHE_VAR_TYPE}=\"${${CACHE_VAR}}\"")
  endif()
endforeach(CACHE_VAR ${CACHE_VARS})

# Record CMake command line
set(CMAKE_INVOKE "${CMAKE_COMMAND} ${CMAKE_INVOKE_ARGS} ${CMAKE_CURRENT_SOURCE_DIR}"
  CACHE STRING "Command used to invoke cmake" FORCE)

# Create a shell script that reinvokes CMake with the captured command line
if (NOT ${CMAKE_GENERATOR} STREQUAL "Unix Makefiles")
  set(RECMAKE_GENERATOR "-G ${CMAKE_GENERATOR}")
endif()
set(RECMAKE_INITIAL_FILE ${CMAKE_BINARY_DIR}/recmake_initial.sh)
set(RECMAKE_REPLAY_FILE ${CMAKE_BINARY_DIR}/recmake_replay.sh)

if (NOT EXISTS ${RECMAKE_INITIAL_FILE})
  file(WRITE ${RECMAKE_INITIAL_FILE}
    "#!/bin/sh\nrm -f CMakeCache.txt\n"
    "${CMAKE_INVOKE} ${RECMAKE_GENERATOR}\n")
endif()

if (EXISTS ${RECMAKE_REPLAY_FILE})
  file(APPEND ${RECMAKE_REPLAY_FILE} "${CMAKE_INVOKE}\n")
else()
  file(WRITE ${RECMAKE_REPLAY_FILE}
    "#!/bin/sh\nrm -f CMakeCache.txt\n"
    "${CMAKE_INVOKE} ${RECMAKE_GENERATOR}\n")
endif()
