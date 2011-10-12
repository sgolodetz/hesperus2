##########################################
# Top-level CMakeLists.txt for hesperus2 #
##########################################

cmake_minimum_required(VERSION 2.6)

PROJECT(hesperus2)

OPTION(BUILD_ENGINE "Build the engine?" ON)
IF(BUILD_ENGINE)
	ADD_SUBDIRECTORY(engine)
ENDIF(BUILD_ENGINE)

OPTION(BUILD_GAMES "Build the games?" ON)
IF(BUILD_GAMES)
	ADD_SUBDIRECTORY(games)
ENDIF(BUILD_GAMES)

OPTION(BUILD_TESTS "Build the tests?" OFF)
IF(BUILD_TESTS)
	ADD_SUBDIRECTORY(tests)
ENDIF(BUILD_TESTS)
