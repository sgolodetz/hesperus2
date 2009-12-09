#############################################
# CMakeLists.txt for games/ScarletPimpernel #
#############################################

###########################
# Specify the target name #
###########################

SET(targetname ScarletPimpernel)

#############################
# Specify the project files #
#############################

##
SET(ScarletPimpernel_sources main.cpp)

##
SET(game_sources
game/Game.cpp
game/GameData.cpp
game/GameState.cpp
game/GameState_Level.cpp
game/GameState_LoadLevel.cpp
game/GameState_MainMenu.cpp
game/GameTransition_LevelLoaded.cpp
game/GameTransition_NewGame.cpp
)

SET(game_headers
game/Game.h
game/GameData.h
game/GameState.h
game/GameState_Level.h
game/GameState_LoadLevel.h
game/GameState_MainMenu.h
game/GameTransition_LevelLoaded.h
game/GameTransition_NewGame.h
)

##
SET(definitions
${hesperus2_SOURCE_DIR}/../resources/ScarletPimpernel/definitions/test-def.xml
)

##
SET(scripts
${hesperus2_SOURCE_DIR}/../resources/ScarletPimpernel/scripts/config.as
${hesperus2_SOURCE_DIR}/../resources/ScarletPimpernel/scripts/guard-ai.as
)

##############################################################
# Collect the project files into sources, headers and others #
##############################################################

SET(sources ${ScarletPimpernel_sources} ${game_sources})
SET(headers ${game_headers})
SET(others ${definitions} ${scripts})

#############################
# Specify the source groups #
#############################

##
SOURCE_GROUP(.cpp FILES ${sources})

##
SOURCE_GROUP(game\\.cpp FILES ${game_sources})
SOURCE_GROUP(game\\.h FILES ${game_headers})

##
SOURCE_GROUP(resources\\definitions FILES ${definitions})

##
SOURCE_GROUP(resources\\scripts FILES ${scripts})

###################################
# Specify the include directories #
###################################

INCLUDE_DIRECTORIES(${hesperus2_SOURCE_DIR}/engine/core)

################################
# Specify the libraries to use #
################################

INCLUDE(${hesperus2_SOURCE_DIR}/UseASX.cmake)
INCLUDE(${hesperus2_SOURCE_DIR}/UseBoost.cmake)
INCLUDE(${hesperus2_SOURCE_DIR}/UseFMOD.cmake)
INCLUDE(${hesperus2_SOURCE_DIR}/UseGLEW.cmake)
INCLUDE(${hesperus2_SOURCE_DIR}/UseLodePNG.cmake)
INCLUDE(${hesperus2_SOURCE_DIR}/UsePropParser.cmake)
INCLUDE(${hesperus2_SOURCE_DIR}/UseSDL.cmake)

##########################################
# Specify the target and where to put it #
##########################################

INCLUDE(${hesperus2_SOURCE_DIR}/SetGameTarget.cmake)

#################################
# Specify the libraries to link #
#################################

TARGET_LINK_LIBRARIES(${targetname} hesperus)
INCLUDE(${hesperus2_SOURCE_DIR}/LinkASX.cmake)
INCLUDE(${hesperus2_SOURCE_DIR}/LinkBoost.cmake)
INCLUDE(${hesperus2_SOURCE_DIR}/LinkFMOD.cmake)
INCLUDE(${hesperus2_SOURCE_DIR}/LinkGLEW.cmake)
INCLUDE(${hesperus2_SOURCE_DIR}/LinkLodePNG.cmake)
INCLUDE(${hesperus2_SOURCE_DIR}/LinkPropParser.cmake)
INCLUDE(${hesperus2_SOURCE_DIR}/LinkSDL.cmake)

#############################
# Specify things to install #
#############################

INCLUDE(${hesperus2_SOURCE_DIR}/InstallGame.cmake)
