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

###############################
# Specify the necessary paths #
###############################

INCLUDE_DIRECTORIES(
${hesperus2_SOURCE_DIR}/../libraries/angelscript_2.17.2/sdk/angelscript/include
)

LINK_DIRECTORIES(
${hesperus2_SOURCE_DIR}/../libraries/angelscript_2.17.2/sdk/angelscript/lib
)

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

###########################################
# Specify the necessary libraries to link #
###########################################

TARGET_LINK_LIBRARIES(${targetname} debug angelscriptd.lib optimized angelscript.lib)

#############################
# Specify things to install #
#############################

INSTALL(TARGETS ${targetname} DESTINATION bin/tests/${targetname}/bin)
