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
SET(images_sources
hesp/images/PNGSaver.cpp
)

SET(images_headers
hesp/images/Image.h
hesp/images/PixelTypes.h
hesp/images/PNGSaver.h
hesp/images/SimpleImage.h
)

SET(images_templates
hesp/images/SimpleImage.tpp
)

##
SET(io_files_sources
hesp/io/files/DefinitionsSpecifierFile.cpp
hesp/io/files/LightsFile.cpp
hesp/io/files/OnionPortalsFile.cpp
hesp/io/files/PortalsFile.cpp
hesp/io/files/VisFile.cpp
)

SET(io_files_headers
hesp/io/files/BrushesFile.h
hesp/io/files/DefinitionsSpecifierFile.h
hesp/io/files/GeometryFile.h
hesp/io/files/LightsFile.h
hesp/io/files/LitTreeFile.h
hesp/io/files/OnionPortalsFile.h
hesp/io/files/OnionTreeFile.h
hesp/io/files/PortalsFile.h
hesp/io/files/TreeFile.h
hesp/io/files/VisFile.h
)

SET(io_files_templates
hesp/io/files/BrushesFile.tpp
hesp/io/files/GeometryFile.tpp
hesp/io/files/LitTreeFile.tpp
hesp/io/files/OnionTreeFile.tpp
hesp/io/files/TreeFile.tpp
)

##
SET(io_sections_sources
hesp/io/sections/DefinitionsSpecifierSection.cpp
hesp/io/sections/LightmapPrefixSection.cpp
hesp/io/sections/LightsSection.cpp
hesp/io/sections/OnionTreeSection.cpp
hesp/io/sections/TreeSection.cpp
hesp/io/sections/VisSection.cpp
)

SET(io_sections_headers
hesp/io/sections/DefinitionsSpecifierSection.h
hesp/io/sections/LightmapPrefixSection.h
hesp/io/sections/LightsSection.h
hesp/io/sections/OnionTreeSection.h
hesp/io/sections/PolygonsSection.h
hesp/io/sections/TreeSection.h
hesp/io/sections/VisSection.h
)

SET(io_sections_templates
hesp/io/sections/PolygonsSection.tpp
)

##
SET(io_util_sources
hesp/io/util/DirectoryFinder.cpp
hesp/io/util/FieldIO.cpp
hesp/io/util/LineIO.cpp
)

SET(io_util_headers
hesp/io/util/DirectoryFinder.h
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
SET(level_csg_headers
hesp/level/csg/CSGUtil.h
)

SET(level_csg_templates
hesp/level/csg/CSGUtil.tpp
)

##
SET(level_lighting_sources
hesp/level/lighting/Lightmap.cpp
hesp/level/lighting/LightmapGenerator.cpp
hesp/level/lighting/LightmapGrid.cpp
)

SET(level_lighting_headers
hesp/level/lighting/Light.h
hesp/level/lighting/Lightmap.h
hesp/level/lighting/LightmapGenerator.h
hesp/level/lighting/LightmapGrid.h
)

SET(level_lighting_templates
hesp/level/lighting/LightmapGrid.tpp
)

##
SET(level_portals_sources
hesp/level/portals/OnionPortal.cpp
hesp/level/portals/OnionPortalGenerator.cpp
hesp/level/portals/Portal.cpp
hesp/level/portals/PortalGenerator.cpp
)

SET(level_portals_headers
hesp/level/portals/BasePortalGenerator.h
hesp/level/portals/OnionPortal.h
hesp/level/portals/OnionPortalGenerator.h
hesp/level/portals/Portal.h
hesp/level/portals/PortalGenerator.h
)

SET(level_portals_templates
hesp/level/portals/BasePortalGenerator.tpp
)

##
SET(level_trees_sources
hesp/level/trees/BSPBranch.cpp
hesp/level/trees/BSPLeaf.cpp
hesp/level/trees/BSPNode.cpp
hesp/level/trees/BSPTree.cpp
hesp/level/trees/BSPUtil.cpp
hesp/level/trees/OnionBranch.cpp
hesp/level/trees/OnionLeaf.cpp
hesp/level/trees/OnionNode.cpp
hesp/level/trees/OnionTree.cpp
hesp/level/trees/OnionUtil.cpp
)

SET(level_trees_headers
hesp/level/trees/BSPBranch.h
hesp/level/trees/BSPCompiler.h
hesp/level/trees/BSPLeaf.h
hesp/level/trees/BSPNode.h
hesp/level/trees/BSPTree.h
hesp/level/trees/BSPUtil.h
hesp/level/trees/OnionBranch.h
hesp/level/trees/OnionCompiler.h
hesp/level/trees/OnionLeaf.h
hesp/level/trees/OnionNode.h
hesp/level/trees/OnionTree.h
hesp/level/trees/OnionUtil.h
hesp/level/trees/TreeUtil.h
)

SET(level_trees_templates
hesp/level/trees/BSPCompiler.tpp
hesp/level/trees/BSPUtil.tpp
hesp/level/trees/OnionCompiler.tpp
hesp/level/trees/TreeUtil.tpp
)

##
SET(level_vis_sources
hesp/level/vis/Antipenumbra.cpp
hesp/level/vis/VisCalculator.cpp
)

SET(level_vis_headers
hesp/level/vis/Antipenumbra.h
hesp/level/vis/VisCalculator.h
hesp/level/vis/VisTable.h
)

SET(level_vis_templates
hesp/level/vis/Antipenumbra.tpp
hesp/level/vis/VisTable.tpp
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
${images_sources}
${io_files_sources}
${io_sections_sources}
${io_util_sources}
${level_brushes_sources}
${level_lighting_sources}
${level_portals_sources}
${level_trees_sources}
${level_vis_sources}
${math_sources}
${math_geom_sources}
${math_vectors_sources}
${util_sources}
)

SET(headers
${colours_headers}
${exceptions_headers}
${images_headers}
${io_files_headers}
${io_sections_headers}
${io_util_headers}
${level_brushes_headers}
${level_csg_headers}
${level_lighting_headers}
${level_portals_headers}
${level_trees_headers}
${level_vis_headers}
${math_headers}
${math_geom_headers}
${math_vectors_headers}
${util_headers}
)

SET(templates
${images_templates}
${io_files_templates}
${io_sections_templates}
${io_util_templates}
${level_brushes_templates}
${level_csg_templates}
${level_lighting_templates}
${level_portals_templates}
${level_trees_templates}
${level_vis_templates}
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
SOURCE_GROUP(images\\.cpp FILES ${images_sources})
SOURCE_GROUP(images\\.h FILES ${images_headers})
SOURCE_GROUP(images\\.tpp FILES ${images_templates})

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
SOURCE_GROUP(level\\csg\\.h FILES ${level_csg_headers})
SOURCE_GROUP(level\\csg\\.tpp FILES ${level_csg_templates})

##
SOURCE_GROUP(level\\lighting\\.cpp FILES ${level_lighting_sources})
SOURCE_GROUP(level\\lighting\\.h FILES ${level_lighting_headers})
SOURCE_GROUP(level\\lighting\\.tpp FILES ${level_lighting_templates})

##
SOURCE_GROUP(level\\portals\\.cpp FILES ${level_portals_sources})
SOURCE_GROUP(level\\portals\\.h FILES ${level_portals_headers})
SOURCE_GROUP(level\\portals\\.tpp FILES ${level_portals_templates})

##
SOURCE_GROUP(level\\trees\\.cpp FILES ${level_trees_sources})
SOURCE_GROUP(level\\trees\\.h FILES ${level_trees_headers})
SOURCE_GROUP(level\\trees\\.tpp FILES ${level_trees_templates})

##
SOURCE_GROUP(level\\vis\\.cpp FILES ${level_vis_sources})
SOURCE_GROUP(level\\vis\\.h FILES ${level_vis_headers})
SOURCE_GROUP(level\\vis\\.tpp FILES ${level_vis_templates})

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
INCLUDE(${hesperus2_SOURCE_DIR}/UseLodePNG.cmake)

##########################################
# Specify the target and where to put it #
##########################################

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${hesperus2_BINARY_DIR}/lib)
ADD_LIBRARY(${targetname} STATIC ${sources} ${headers} ${templates})
INCLUDE(${hesperus2_SOURCE_DIR}/VCTargetHack.cmake)

SET_TARGET_PROPERTIES(${targetname} PROPERTIES DEBUG_OUTPUT_NAME "hesperus_d")
