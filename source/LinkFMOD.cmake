##################
# LinkFMOD.cmake #
##################

IF(CMAKE_SIZEOF_VOID_P EQUAL 4)
	# 32-bit
	IF(MSVC_IDE)
		TARGET_LINK_LIBRARIES(${targetname} debug fmodexL_vc optimized fmodex_vc)
	ELSE(MSVC_IDE)
		TARGET_LINK_LIBRARIES(${targetname} debug fmodexL optimized fmodex)
	ENDIF(MSVC_IDE)
ELSE(CMAKE_SIZEOF_VOID_P EQUAL 4)
	# 64-bit
	IF(MSVC_IDE)
		# TODO
	ELSE(MSVC_IDE)
		TARGET_LINK_LIBRARIES(${targetname} debug fmodex64L optimized fmodex64)
	ENDIF(MSVC_IDE)
ENDIF(CMAKE_SIZEOF_VOID_P EQUAL 4)
