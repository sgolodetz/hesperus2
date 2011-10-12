############################################
# CMakeLists.txt for tests/test-asx-2.17.2 #
############################################

###########################
# Specify the target name #
###########################

SET(targetname test-asx-2.17.2)

#############################
# Specify the project files #
#############################

SET(sources main.cpp)

SET(scripts
ScriptA.as
ScriptC.as
sub/ScriptB.as
)

#############################
# Specify the source groups #
#############################

SOURCE_GROUP(.cpp FILES ${sources})
SOURCE_GROUP(.as FILES ${scripts})

################################
# Specify the libraries to use #
################################

INCLUDE(${hesperus2_SOURCE_DIR}/UseASX.cmake)

##########################################
# Specify the target and where to put it #
##########################################

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${hesperus2_BINARY_DIR}/bin/tests/${targetname}/bin)
ADD_EXECUTABLE(${targetname} ${sources} ${scripts})
INCLUDE(${hesperus2_SOURCE_DIR}/VCTargetHack.cmake)

#################################
# Specify the libraries to link #
#################################

INCLUDE(${hesperus2_SOURCE_DIR}/LinkASX.cmake)

##############################
# Specify and copy resources #
##############################

SET(resources ${scripts})
INCLUDE(${hesperus2_SOURCE_DIR}/CopyResources.cmake)

#############################
# Specify things to install #
#############################

INCLUDE(${hesperus2_SOURCE_DIR}/InstallTest.cmake)
INSTALL(FILES ScriptA.as ScriptC.as DESTINATION bin/tests/${targetname}/resources)
INSTALL(FILES sub/ScriptB.as DESTINATION bin/tests/${targetname}/resources/sub)
