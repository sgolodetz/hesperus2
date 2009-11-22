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

################################
# Specify the libraries to use #
################################

INCLUDE(${hesperus2_SOURCE_DIR}/UseAngelScript.cmake)

##########################################
# Specify the target and where to put it #
##########################################

INCLUDE(${hesperus2_SOURCE_DIR}/SetTarget.cmake)

#################################
# Specify the libraries to link #
#################################

INCLUDE(${hesperus2_SOURCE_DIR}/LinkAngelScript.cmake)

#############################
# Specify things to install #
#############################

INSTALL(TARGETS ${targetname} DESTINATION bin/tests/${targetname}/bin)
