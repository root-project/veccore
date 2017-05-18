# Try to capture the initial set of cmake command line args passed by
# the user for configuration.
# Original Recipe taken from http://stackoverflow.com/questions/10205986/how-to-capture-cmake-command-line-arguments
# and adapted in ROOT and GeantV
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
#
GET_CMAKE_PROPERTY(CACHE_VARS CACHE_VARIABLES)
FOREACH(CACHE_VAR ${CACHE_VARS})
    GET_PROPERTY(CACHE_VAR_HELPSTRING CACHE ${CACHE_VAR} PROPERTY HELPSTRING)
    IF(CACHE_VAR_HELPSTRING STREQUAL "No help, variable specified on the command line.")
        GET_PROPERTY(CACHE_VAR_TYPE CACHE ${CACHE_VAR} PROPERTY TYPE)
        IF(CACHE_VAR_TYPE STREQUAL "UNINITIALIZED")
            SET(CACHE_VAR_TYPE)
        ELSE(CACHE_VAR_TYPE STREQUAL "UNINITIALIZED")
            SET(CACHE_VAR_TYPE :${CACHE_VAR_TYPE})
        ENDIF()
        SET(CMAKE_INVOKE_ARGS "${CMAKE_INVOKE_ARGS} -D${CACHE_VAR}${CACHE_VAR_TYPE}=\"${${CACHE_VAR}}\"")
    ENDIF()
ENDFOREACH(CACHE_VAR ${CACHE_VARS})
# Record the full command line invocation.
SET(CMAKE_INVOKE "${CMAKE_COMMAND} ${CMAKE_INVOKE_ARGS} ${CMAKE_CURRENT_SOURCE_DIR}" CACHE STRING "Command used to invoke cmake" FORCE)
# Create a simple shell script that allows us to reinvoke cmake with the captured command line.
IF (NOT WIN32)
    if (NOT ${CMAKE_GENERATOR} STREQUAL "Unix Makefiles")
        set(RECMAKE_GENERATOR "-G ${CMAKE_GENERATOR}")
    endif()
    SET(RECMAKE_REPLAY_FILE ${CMAKE_BINARY_DIR}/recmake_replay.sh)
    SET(RECMAKE_INITIAL_FILE ${CMAKE_BINARY_DIR}/recmake_initial.sh)
    if (NOT EXISTS ${RECMAKE_INITIAL_FILE})
        FILE(WRITE ${RECMAKE_INITIAL_FILE} "#!/bin/sh\n"
                "rm -f CMakeCache.txt\n"
                "${CMAKE_INVOKE} ${RECMAKE_GENERATOR}\n")
    endif()
    if (EXISTS ${RECMAKE_REPLAY_FILE})
        FILE(APPEND ${RECMAKE_REPLAY_FILE} "${CMAKE_INVOKE}\n")
    else()
        FILE(WRITE ${RECMAKE_REPLAY_FILE} "#!/bin/sh\n"
            "rm -f CMakeCache.txt\n"
            "${CMAKE_INVOKE} ${RECMAKE_GENERATOR}\n")
    endif()
ENDIF (NOT WIN32)
