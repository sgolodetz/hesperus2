/***
 * hlight: main.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include <iostream>
#include <string>
#include <vector>

#include <boost/lexical_cast.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <hesp/images/PNGSaver.h>
#include <hesp/io/files/LightsFile.h>
#include <hesp/io/files/LitTreeFile.h>
#include <hesp/io/files/TreeFile.h>
#include <hesp/io/files/VisFile.h>
#include <hesp/level/lighting/Lightmap.h>
#include <hesp/level/lighting/LightmapGenerator.h>
#include <hesp/util/PolygonTypes.h>
using namespace hesp;

//#################### FUNCTIONS ####################
void quit_with_error(const std::string& error)
{
	std::cout << "Error: " << error << std::endl;
	exit(EXIT_FAILURE);
}

void quit_with_usage()
{
	std::cout << "Usage: hlight <input tree> <input vis> <input lights> <lightmap file prefix> <output filename>" << std::endl;
	exit(EXIT_FAILURE);
}

void run_generator(const std::string& treeFilename, const std::string& visFilename, const std::string& lightsFilename,
				   const std::string& lightmapPrefix, const std::string& outputFilename)
try		// <--- Note the "function try" syntax (this is a rarely-used C++ construct).
{
	// Read in the polygons and tree.
	std::vector<TexturedPolygon_Ptr> polygons;
	BSPTree_Ptr tree;
	TreeFile::load(treeFilename, polygons, tree);

	// Read in the vis table.
	LeafVisTable_Ptr leafVis = VisFile::load(visFilename);

	// Read in the lights.
	std::vector<Light> lights = LightsFile::load(lightsFilename);

	// Generate the lit polygons and lightmaps.
	LightmapGenerator lg(polygons, lights, tree, leafVis);
	lg.generate_lightmaps();

	typedef std::vector<Lightmap_Ptr> LightmapVector;
	typedef shared_ptr<const LightmapVector> LightmapVector_CPtr;
	typedef std::vector<TexturedLitPolygon_Ptr> TexLitPolyVector;
	typedef shared_ptr<const TexLitPolyVector> TexLitPolyVector_CPtr;

	TexLitPolyVector_CPtr litPolygons = lg.lit_polygons();
	LightmapVector_CPtr lightmaps = lg.lightmaps();

	// Write the lit polygons, tree and lightmap prefix to the output file.
	LitTreeFile::save(outputFilename, *litPolygons, tree, lightmapPrefix);

	// Write the lightmaps out as 24-bit bitmaps.
	int lightmapCount = static_cast<int>(lightmaps->size());
	for(int i=0; i<lightmapCount; ++i)
	{
		std::string lightmapFilename = lightmapPrefix + lexical_cast<std::string>(i) + ".png";
		Image24_Ptr image = (*lightmaps)[i]->to_image();
		PNGSaver::save_image24(lightmapFilename, image);
	}
}
catch(Exception& e) { quit_with_error(e.cause()); }

int main(int argc, char *argv[])
{
	if(argc != 6) quit_with_usage();
	std::vector<std::string> args(argv, argv + argc);
	run_generator(args[1], args[2], args[3], args[4], args[5]);
	return 0;
}
