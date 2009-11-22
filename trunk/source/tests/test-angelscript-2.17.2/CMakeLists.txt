####################################################
# CMakeLists.txt for tests/test-angelscript-2.17.2 #
####################################################

###########################
# Specify the target name #
###########################

SET(targetname test-angelscript-2.17.2)

#############################
# Specify the project files #
#############################

SET(sources main.cpp)

#############################
# Specify the source groups #
#############################

SOURCE_GROUP(.cpp FILES ${sources})

##########################################
# Specify the target and where to put it #
##########################################

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${hesperus2_BINARY_DIR}/bin/tests/${targetname}/bin)
ADD_EXECUTABLE(${targetname} ${sources})
IF(MSVC_IDE)
	# A hack to get around the "Debug" and "Release" directories Visual Studio tries to add
	SET_TARGET_PROPERTIES(${targetname} PROPERTIES PREFIX "../")
	SET_TARGET_PROPERTIES(${targetname} PROPERTIES IMPORT_PREFIX "../")
ENDIF(MSVC_IDE)

#############################
# Specify things to install #
#############################

INSTALL(TARGETS ${targetname} DESTINATION bin/tests/${targetname}/bin)
