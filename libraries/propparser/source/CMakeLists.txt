###########################################
# Top-level CMakeLists.txt for propparser #
###########################################

cmake_minimum_required(VERSION 2.6)

PROJECT(propparser)

###########################
# Specify the target name #
###########################

SET(targetname propparser)

#######################
# Find Flex and Bison #
#######################

FIND_PACKAGE(BISON REQUIRED)
FIND_PACKAGE(FLEX REQUIRED)

###############################
# Create the parser and lexer #
###############################

BISON_TARGET(Parser yypropsimplifier.y ${CMAKE_CURRENT_BINARY_DIR}/yypropsimplifier.tab.c COMPILE_FLAGS "-pyypropsimplifier")
FLEX_TARGET(Lexer yypropsimplifier.l ${CMAKE_CURRENT_BINARY_DIR}/lex.yypropsimplifier.c COMPILE_FLAGS "-Pyypropsimplifier")
ADD_FLEX_BISON_DEPENDENCY(Lexer Parser)

#############################
# Specify the project files #
#############################

SET(sources PropSimplifier.cpp)
SET(headers PropSimplifier.h)
SET(others yypropsimplifier.l yypropsimplifier.y ${BISON_Parser_OUTPUTS} ${FLEX_Lexer_OUTPUTS})

#############################
# Specify the source groups #
#############################

SOURCE_GROUP(.cpp FILES ${sources})
SOURCE_GROUP(.h FILES ${headers})
SOURCE_GROUP(others FILES ${others})

##########################################
# Specify the target and where to put it #
##########################################

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${propparser_BINARY_DIR}/lib)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${propparser_BINARY_DIR}/lib)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${propparser_BINARY_DIR}/lib)
ADD_LIBRARY(${targetname} STATIC ${sources} ${headers} ${others})

SET_TARGET_PROPERTIES(${targetname} PROPERTIES DEBUG_OUTPUT_NAME "propparser_d")

################################
# Specify extra compiler flags #
################################

IF(MSVC_IDE)
	# Allow extern "C" functions to throw exceptions
	SET_TARGET_PROPERTIES(${targetname} PROPERTIES COMPILE_FLAGS "/EHsc-")
ENDIF(MSVC_IDE)

#############################
# Specify things to install #
#############################

INSTALL(TARGETS ${targetname} DESTINATION lib)
