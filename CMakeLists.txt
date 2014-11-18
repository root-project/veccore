#
# When moved-out of VecGeom, needs to add proper set of options, and global definitions. 
#
# project(VecCore)
#

# Install headers and libraries

# trailing slash after inc is semantically essential, it means to copy the content
# of inc into the target directory (.../VecCore) rather than into .../VecCore/inc
install(DIRECTORY inc/ DESTINATION ${INSTALL_INCLUDE_DIR}/VecCore )

#install(TARGETS veccore DESTINATION ${INSTALL_LIB_DIR})
#if (CUDA)
#  install(TARGETS veccorecuda DESTINATION ${INSTALL_LIB_DIR})
#endif()
