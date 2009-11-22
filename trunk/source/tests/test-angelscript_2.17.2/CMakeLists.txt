####################################################
# CMakeLists.txt for tests/test-angelscript_2.17.2 #
####################################################

###########################
# Specify the target name #
###########################

SET(targetname test-angelscript_2.17.2)

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

INCLUDE(${hesperus2_SOURCE_DIR}/SetTarget.cmake)

###########################################
# Specify the necessary libraries to link #
###########################################

TARGET_LINK_LIBRARIES(${targetname} debug angelscriptd optimized angelscript)

#############################
# Specify things to install #
#############################

INSTALL(TARGETS ${targetname} DESTINATION bin/tests/${targetname}/bin)
