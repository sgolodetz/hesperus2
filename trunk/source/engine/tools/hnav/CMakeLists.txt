########################################
# CMakeLists.txt for engine/tools/hnav #
########################################

###########################
# Specify the target name #
###########################

SET(targetname hnav)

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

INCLUDE(${hesperus2_SOURCE_DIR}/UseASX.cmake)
INCLUDE(${hesperus2_SOURCE_DIR}/UseBoost.cmake)
INCLUDE(${hesperus2_SOURCE_DIR}/UseLodePNG.cmake)
INCLUDE(${hesperus2_SOURCE_DIR}/UsePropParser.cmake)
INCLUDE(${hesperus2_SOURCE_DIR}/UseSDL.cmake)

##########################################
# Specify the target and where to put it #
##########################################

INCLUDE(${hesperus2_SOURCE_DIR}/SetToolTarget.cmake)

#################################
# Specify the libraries to link #
#################################

TARGET_LINK_LIBRARIES(${targetname} hesperus)
INCLUDE(${hesperus2_SOURCE_DIR}/LinkASX.cmake)
INCLUDE(${hesperus2_SOURCE_DIR}/LinkBoost.cmake)
INCLUDE(${hesperus2_SOURCE_DIR}/LinkLodePNG.cmake)
INCLUDE(${hesperus2_SOURCE_DIR}/LinkPropParser.cmake)
INCLUDE(${hesperus2_SOURCE_DIR}/LinkSDL.cmake)

#############################
# Specify things to install #
#############################

INCLUDE(${hesperus2_SOURCE_DIR}/InstallTool.cmake)
