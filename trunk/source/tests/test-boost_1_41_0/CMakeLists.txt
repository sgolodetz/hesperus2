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

##############
# Find Boost #
##############

INCLUDE(${hesperus2_SOURCE_DIR}/UseBoost.cmake)

##########################################
# Specify the target and where to put it #
##########################################

INCLUDE(${hesperus2_SOURCE_DIR}/SetTarget.cmake)

###########################################
# Specify the necessary libraries to link #
###########################################

TARGET_LINK_LIBRARIES(${targetname} ${Boost_LIBRARIES})

#############################
# Specify things to install #
#############################

INSTALL(TARGETS ${targetname} DESTINATION bin/tests/${targetname}/bin)
