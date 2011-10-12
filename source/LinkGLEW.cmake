##################
# LinkGLEW.cmake #
##################

IF(MSVC_IDE)
	TARGET_LINK_LIBRARIES(${targetname} glew32)
ELSE(MSVC_IDE)
	TARGET_LINK_LIBRARIES(${targetname} GLEW)
ENDIF(MSVC_IDE)
