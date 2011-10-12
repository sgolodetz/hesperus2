#####################################
# CMakeLists.txt for tests/test-hsm #
#####################################

###########################
# Specify the target name #
###########################

SET(targetname test-hsm)

#############################
# Specify the project files #
#############################

SET(sources main.cpp)

#############################
# Specify the source groups #
#############################

SOURCE_GROUP(.cpp FILES ${sources})

###################################
# Specify the include directories #
###################################

INCLUDE_DIRECTORIES(${hesperus2_SOURCE_DIR}/engine/core)

################################
# Specify the libraries to use #
################################

INCLUDE(${hesperus2_SOURCE_DIR}/UseBoost.cmake)

##########################################
# Specify the target and where to put it #
##########################################

INCLUDE(${hesperus2_SOURCE_DIR}/SetTestTarget.cmake)

#################################
# Specify the libraries to link #
#################################

TARGET_LINK_LIBRARIES(${targetname} hesperus)

#############################
# Specify things to install #
#############################

INCLUDE(${hesperus2_SOURCE_DIR}/InstallTest.cmake)
