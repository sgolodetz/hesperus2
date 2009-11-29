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
SET(colours_sources
hesp/colours/Colour3d.cpp
)

SET(colours_headers
hesp/colours/Colour3d.h
)

##
SET(exceptions_headers
hesp/exceptions/Exception.h
hesp/exceptions/InvalidParameterException.h
)

##
SET(io_files_sources
hesp/io/files/DefinitionsSpecifierFile.cpp
hesp/io/files/LightsFile.cpp
)

SET(io_files_headers
hesp/io/files/BrushesFile.h
hesp/io/files/DefinitionsSpecifierFile.h
hesp/io/files/GeometryFile.h
hesp/io/files/LightsFile.h
hesp/io/files/TreeFile.h
)

SET(io_files_templates
hesp/io/files/BrushesFile.tpp
hesp/io/files/GeometryFile.tpp
hesp/io/files/TreeFile.tpp
)

##
SET(io_sections_sources
hesp/io/sections/DefinitionsSpecifierSection.cpp
hesp/io/sections/LightsSection.cpp
hesp/io/sections/TreeSection.cpp
)

SET(io_sections_headers
hesp/io/sections/DefinitionsSpecifierSection.h
hesp/io/sections/LightsSection.h
hesp/io/sections/PolygonsSection.h
hesp/io/sections/TreeSection.h
)

SET(io_sections_templates
hesp/io/sections/PolygonsSection.tpp
)

##
SET(io_util_sources
hesp/io/util/FieldIO.cpp
hesp/io/util/LineIO.cpp
)

SET(io_util_headers
hesp/io/util/FieldIO.h
hesp/io/util/IOUtil.h
hesp/io/util/LineIO.h
)

SET(io_util_templates
hesp/io/util/FieldIO.tpp
hesp/io/util/IOUtil.tpp
)

##
SET(level_brushes_sources
hesp/level/brushes/BrushFunction.cpp
)

SET(level_brushes_headers
hesp/level/brushes/BrushFunction.h
hesp/level/brushes/PolyhedralBrush.h
)

SET(level_brushes_templates
hesp/level/brushes/PolyhedralBrush.tpp
)

##
SET(level_lighting_headers
hesp/level/lighting/Light.h
)

##
SET(level_trees_sources
hesp/level/trees/BSPBranch.cpp
hesp/level/trees/BSPLeaf.cpp
hesp/level/trees/BSPNode.cpp
hesp/level/trees/BSPTree.cpp
hesp/level/trees/BSPUtil.cpp
)

SET(level_trees_headers
hesp/level/trees/BSPBranch.h
hesp/level/trees/BSPCompiler.h
hesp/level/trees/BSPLeaf.h
hesp/level/trees/BSPNode.h
hesp/level/trees/BSPTree.h
hesp/level/trees/BSPUtil.h
)

SET(level_trees_templates
hesp/level/trees/BSPCompiler.tpp
hesp/level/trees/BSPUtil.tpp
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
hesp/math/vectors/TexCoords.h
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
${colours_sources}
${io_files_sources}
${io_sections_sources}
${io_util_sources}
${level_brushes_sources}
${level_trees_sources}
${math_sources}
${math_geom_sources}
${math_vectors_sources}
${util_sources}
)

SET(headers
${colours_headers}
${exceptions_headers}
${io_files_headers}
${io_sections_headers}
${io_util_headers}
${level_brushes_headers}
${level_lighting_headers}
${level_trees_headers}
${math_headers}
${math_geom_headers}
${math_vectors_headers}
${util_headers}
)

SET(templates
${io_files_templates}
${io_sections_templates}
${io_util_templates}
${level_brushes_templates}
${level_trees_templates}
${math_geom_templates}
)

#############################
# Specify the source groups #
#############################

##
SOURCE_GROUP(colours\\.cpp FILES ${colours_sources})
SOURCE_GROUP(colours\\.h FILES ${colours_headers})

##
SOURCE_GROUP(exceptions\\.h FILES ${exceptions_headers})

##
SOURCE_GROUP(io\\files\\.cpp FILES ${io_files_sources})
SOURCE_GROUP(io\\files\\.h FILES ${io_files_headers})
SOURCE_GROUP(io\\files\\.tpp FILES ${io_files_templates})

##
SOURCE_GROUP(io\\sections\\.cpp FILES ${io_sections_sources})
SOURCE_GROUP(io\\sections\\.h FILES ${io_sections_headers})
SOURCE_GROUP(io\\sections\\.tpp FILES ${io_sections_templates})

##
SOURCE_GROUP(io\\util\\.cpp FILES ${io_util_sources})
SOURCE_GROUP(io\\util\\.h FILES ${io_util_headers})
SOURCE_GROUP(io\\util\\.tpp FILES ${io_util_templates})

##
SOURCE_GROUP(level\\brushes\\.cpp FILES ${level_brushes_sources})
SOURCE_GROUP(level\\brushes\\.h FILES ${level_brushes_headers})
SOURCE_GROUP(level\\brushes\\.tpp FILES ${level_brushes_templates})

##
SOURCE_GROUP(level\\lighting\\.h FILES ${level_lighting_headers})

##
SOURCE_GROUP(level\\trees\\.cpp FILES ${level_trees_sources})
SOURCE_GROUP(level\\trees\\.h FILES ${level_trees_headers})
SOURCE_GROUP(level\\trees\\.tpp FILES ${level_trees_templates})

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
