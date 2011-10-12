#################
# LinkSDL.cmake #
#################

TARGET_LINK_LIBRARIES(${targetname} SDLmain SDL)
IF(UNIX)
	TARGET_LINK_LIBRARIES(${targetname} pthread)
ENDIF(UNIX)

INCLUDE(${hesperus2_SOURCE_DIR}/LinkOpenGL.cmake)
