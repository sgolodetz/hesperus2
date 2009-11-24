##################
# LinkFMOD.cmake #
##################

IF(MSVC_IDE)
	TARGET_LINK_LIBRARIES(${targetname} debug fmodexL_vc optimized fmodex_vc)
ENDIF(MSVC_IDE)
