###########################################
# Top-level CMakeLists.txt for asx-2.17.2 #
###########################################

cmake_minimum_required(VERSION 2.8)

PROJECT(asx-2.17.2)

###########################
# Specify the target name #
###########################

SET(targetname asx)

#############################
# Specify the project files #
#############################

SET(asx_sources
ASXContext.cpp
ASXEngine.cpp
ASXException.cpp
ASXMessage.cpp
ASXModule.cpp
)

SET(asx_headers
../include/angelscript.h
../include/ASXContext.h
../include/ASXEngine.h
../include/ASXException.h
../include/ASXFunction.h
../include/ASXGetReturnValue.h
../include/ASXMessage.h
../include/ASXMethodTypeString.h
../include/ASXModule.h
../include/ASXRefType.h
../include/ASXSetArgValue.h
../include/ASXStructorTypeString.h
../include/ASXTypeString.h
../include/ASXVariable.h
)

SET(asx_templates
../include/ASXEngine.tpp
../include/ASXModule.tpp
)

SET(add_on_scriptbuilder_sources add_on/scriptbuilder/scriptbuilder.cpp)
SET(add_on_scriptbuilder_headers add_on/scriptbuilder/scriptbuilder.h)

SET(add_on_scriptstdstring_sources add_on/scriptstdstring/scriptstdstring.cpp)
SET(add_on_scriptstdstring_headers add_on/scriptstdstring/scriptstdstring.h)

#################################################################
# Collect the project files into sources, headers and templates #
#################################################################

SET(sources ${asx_sources} ${add_on_scriptbuilder_sources} ${add_on_scriptstdstring_sources})
SET(headers ${asx_headers} ${add_on_scriptbuilder_headers} ${add_on_scriptstdstring_headers})
SET(templates ${asx_templates})

#############################
# Specify the source groups #
#############################

SOURCE_GROUP(.cpp FILES ${asx_sources})
SOURCE_GROUP(.h FILES ${asx_headers})
SOURCE_GROUP(.tpp FILES ${asx_templates})

SOURCE_GROUP(add_on\\scriptbuilder\\.cpp ${add_on_scriptbuilder_sources})
SOURCE_GROUP(add_on\\scriptbuilder\\.h ${add_on_scriptbuilder_headers})

SOURCE_GROUP(add_on\\scriptstdstring\\.cpp ${add_on_scriptstdstring_sources})
SOURCE_GROUP(add_on\\scriptstdstring\\.h ${add_on_scriptstdstring_headers})

###############################
# Specify the necessary paths #
###############################

SET(Boost_Include "../../boost_1_41_0/include/boost-1_41" CACHE PATH "The location of the Boost include files")

INCLUDE_DIRECTORIES(
${Boost_Include}
../include
add_on/scriptbuilder
add_on/scriptstdstring
)

##########################################
# Specify the target and where to put it #
##########################################

SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${asx-2.17.2_BINARY_DIR}/lib)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${asx-2.17.2_BINARY_DIR}/lib)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${asx-2.17.2_BINARY_DIR}/lib)
ADD_LIBRARY(${targetname} STATIC ${sources} ${headers} ${templates})

SET_TARGET_PROPERTIES(${targetname} PROPERTIES DEBUG_OUTPUT_NAME "asx_d")

#############################
# Specify things to install #
#############################

INSTALL(TARGETS ${targetname} DESTINATION lib)
