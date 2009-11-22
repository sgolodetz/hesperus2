##############################################
# CMakeLists.txt for tests/test-boost_1_41_0 #
##############################################

###########################
# Specify the target name #
###########################

SET(targetname test-boost_1_41_0)

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

INCLUDE(${hesperus2_SOURCE_DIR}/UseBoost.cmake)

##########################################
# Specify the target and where to put it #
##########################################

INCLUDE(${hesperus2_SOURCE_DIR}/SetTarget.cmake)

#################################
# Specify the libraries to link #
#################################

INCLUDE(${hesperus2_SOURCE_DIR}/LinkBoost.cmake)

#############################
# Specify things to install #
#############################

INSTALL(TARGETS ${targetname} DESTINATION bin/tests/${targetname}/bin)
