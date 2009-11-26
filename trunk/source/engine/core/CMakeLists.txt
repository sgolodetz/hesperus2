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
hesp/exceptions/InvalidParameterException.h
)

##
SET(math_sources
hesp/math/Constants.cpp
)

SET(math_headers
hesp/math/Constants.h
)

##
SET(math_geom_sources
hesp/math/geom/AABB.cpp
hesp/math/geom/GeomUtil.cpp
hesp/math/geom/Plane.cpp
hesp/math/geom/Sphere.cpp
hesp/math/geom/UniquePlanePred.cpp
)

SET(math_geom_headers
hesp/math/geom/AABB.h
hesp/math/geom/GeomUtil.h
hesp/math/geom/LineSegment.h
hesp/math/geom/Plane.h
hesp/math/geom/Polygon.h
hesp/math/geom/Sphere.h
hesp/math/geom/UniquePlanePred.h
)

SET(math_geom_templates
hesp/math/geom/GeomUtil.tpp
hesp/math/geom/Polygon.tpp
)

##
SET(math_vectors_sources
hesp/math/vectors/TexturedLitVector3d.cpp
hesp/math/vectors/TexturedVector3d.cpp
hesp/math/vectors/Vector2d.cpp
hesp/math/vectors/Vector3.cpp
)

SET(math_vectors_headers
hesp/math/vectors/TexturedLitVector3d.h
hesp/math/vectors/TexturedVector3d.h
hesp/math/vectors/Vector2d.h
hesp/math/vectors/Vector3.h
)

##
SET(util_sources
hesp/util/PolygonTypes.cpp
)

SET(util_headers
hesp/util/PolygonTypes.h
)

#################################################################
# Collect the project files into sources, headers and templates #
#################################################################

SET(sources
${math_sources}
${math_geom_sources}
${math_vectors_sources}
${util_sources}
)

SET(headers
${exceptions_headers}
${math_headers}
${math_geom_headers}
${math_vectors_headers}
${util_headers}
)

SET(templates
${math_geom_templates}
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
SOURCE_GROUP(math\\geom\\.cpp FILES ${math_geom_sources})
SOURCE_GROUP(math\\geom\\.h FILES ${math_geom_headers})
SOURCE_GROUP(math\\geom\\.tpp FILES ${math_geom_templates})

##
SOURCE_GROUP(math\\vectors\\.cpp FILES ${math_vectors_sources})
SOURCE_GROUP(math\\vectors\\.h FILES ${math_vectors_headers})

##
SOURCE_GROUP(util\\.cpp FILES ${util_sources})
SOURCE_GROUP(util\\.h FILES ${util_headers})

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

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${hesperus2_BINARY_DIR}/lib)
ADD_LIBRARY(${targetname} STATIC ${sources} ${headers} ${templates})
INCLUDE(${hesperus2_SOURCE_DIR}/VCTargetHack.cmake)

SET_TARGET_PROPERTIES(${targetname} PROPERTIES DEBUG_OUTPUT_NAME "hesperus_d")

#############################
# Specify things to install #
#############################

INSTALL(TARGETS ${targetname} DESTINATION lib)
