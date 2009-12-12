/***
 * hexpand: main.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <boost/filesystem/operations.hpp>
#include <boost/lexical_cast.hpp>
namespace bf = boost::filesystem;
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <hesp/bounds/BoundsManager.h>
#include <hesp/brushes/BrushExpander.h>
#include <hesp/io/files/BrushesFile.h>
#include <hesp/io/files/DefinitionsFile.h>
#include <hesp/io/files/DefinitionsSpecifierFile.h>
#include <hesp/io/util/DirectoryFinder.h>
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
	std::cout << "Usage: hexpand <input definitions specifier> <input brushes>" << std::endl;
	exit(EXIT_FAILURE);
}

void run_expander(const std::string& definitionsSpecifierFilename, const std::string& inputFilename)
{
	// Read in the input definitions specifier.
	std::string definitionsFilename = DefinitionsSpecifierFile::load(definitionsSpecifierFilename);

	// Read in the bounds.
	bf::path definitionsDir = DirectoryFinder::instance().determine_definitions_directory();
	BoundsManager_Ptr boundsManager = DefinitionsFile::load_bounds_only((definitionsDir / definitionsFilename).file_string());

	// Read in the input brushes.
	typedef PolyhedralBrush<CollisionPolygon> ColPolyBrush;
	typedef shared_ptr<ColPolyBrush> ColPolyBrush_Ptr;
	typedef std::vector<ColPolyBrush_Ptr> ColPolyBrushVector;
	ColPolyBrushVector inputBrushes = BrushesFile::load<CollisionPolygon>(inputFilename);

	// Calculate the output stem and extension.
	std::string::size_type k = inputFilename.find('.');
	std::string outputStem;
	if(k == std::string::npos) outputStem = inputFilename + "-";
	else outputStem = inputFilename.substr(0,k) + "-";

	const std::string outputExtension = ".ebr";

	// For each bounds, expand the brushes and write the expanded brushes to file.
	int boundsCount = boundsManager->bounds_count();
	int brushCount = static_cast<int>(inputBrushes.size());
	for(int i=0; i<boundsCount; ++i)
	{
		// Expand the brushes.
		ColPolyBrushVector expandedBrushes(brushCount);
		for(int j=0; j<brushCount; ++j)
		{
			expandedBrushes[j] = BrushExpander::expand_brush(inputBrushes[j], *boundsManager->bounds(i), i);
		}

		// Write the expanded brushes to file.
		std::ostringstream oss;
		oss << outputStem << i << outputExtension;
		BrushesFile::save(oss.str(), expandedBrushes);
	}
}

int main(int argc, char *argv[])
try
{
	if(argc != 3) quit_with_usage();
	std::vector<std::string> args(argv, argv + argc);

	// Set the appropriate resources directory.
	// FIXME: The game to use shouldn't be hard-coded like this.
	DirectoryFinder& finder = DirectoryFinder::instance();
	finder.set_resources_directory(finder.determine_resources_directory_from_tool("ScarletPimpernel"));

	run_expander(args[1], args[2]);
	return 0;
}
catch(Exception& e) { quit_with_error(e.cause()); }
