##################
# UseBoost.cmake #
##################

SET(Boost_ADDITIONAL_VERSIONS "1.41" "1.41.0")
SET(BOOST_ROOT ${hesperus2_SOURCE_DIR}/../libraries/boost_1_41_0)
SET(Boost_USE_STATIC_LIBS ON)
FIND_PACKAGE(Boost 1.41.0 REQUIRED COMPONENTS date_time filesystem system)
IF(Boost_FOUND)
	INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
	LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
ELSE(Boost_FOUND)
	MESSAGE(FATAL_ERROR "Boost not found. Please set the directories manually using the advanced view in CMake.")
ENDIF(Boost_FOUND)
