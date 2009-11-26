##################################
# CMakeLists.txt for engine/core #
##################################

###########################
# Specify the target name #
###########################

SET(targetname hesperus)

#############################
# Specify the project files #
#############################

##
SET(exceptions_headers
hesp/exceptions/Exception.h
)

##
SET(math_sources
hesp/math/Constants.cpp
)

SET(math_headers
hesp/math/Constants.h
)

##
SET(math_vectors_sources
hesp/math/vectors/Vector3.cpp
)

SET(math_vectors_headers
hesp/math/vectors/Vector3.h
)

#################################################################
# Collect the project files into sources, headers and templates #
#################################################################

SET(sources
${math_sources}
${math_vectors_sources}
)

SET(headers
${exceptions_headers}
${math_headers}
${math_vectors_headers}
)

SET(templates
)

#############################
# Specify the source groups #
#############################

##
SOURCE_GROUP(exceptions\\.h FILES ${exceptions_headers})

##
SOURCE_GROUP(math\\.cpp FILES ${math_sources})
SOURCE_GROUP(math\\.h FILES ${math_headers})

##
SOURCE_GROUP(math\\vectors\\.cpp FILES ${math_vectors_sources})
SOURCE_GROUP(math\\vectors\\.h FILES ${math_vectors_headers})

###############################
# Specify the necessary paths #
###############################

INCLUDE_DIRECTORIES(${hesperus2_SOURCE_DIR}/engine/core)

################################
# Specify the libraries to use #
################################

INCLUDE(${hesperus2_SOURCE_DIR}/UseBoost.cmake)

##########################################
# Specify the target and where to put it #
##########################################

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${hesperus2_BINARY_DIR}/lib)
ADD_LIBRARY(${targetname} STATIC ${sources} ${headers} ${templates})
INCLUDE(${hesperus2_SOURCE_DIR}/VCTargetHack.cmake)

SET_TARGET_PROPERTIES(${targetname} PROPERTIES DEBUG_OUTPUT_NAME "hesperus_d")

#############################
# Specify things to install #
#############################

INSTALL(TARGETS ${targetname} DESTINATION lib)
