#########################
# LinkAngelScript.cmake #
#########################

IF(MSVC_IDE)
	TARGET_LINK_LIBRARIES(${targetname} debug angelscriptd optimized angelscript)
ELSE(MSVC_IDE)
	TARGET_LINK_LIBRARIES(${targetname} angelscript)
ENDIF(MSVC_IDE)
