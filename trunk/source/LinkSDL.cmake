#################
# LinkSDL.cmake #
#################

TARGET_LINK_LIBRARIES(${targetname} SDLmain SDL ${OPENGL_LIBRARY})
IF(UNIX)
	TARGET_LINK_LIBRARIES(${targetname} pthread)
ENDIF(UNIX)
