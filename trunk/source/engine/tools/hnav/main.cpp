/***
 * hnav: main.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <iostream>
#include <string>
#include <vector>

#include <boost/filesystem/operations.hpp>
namespace bf = boost::filesystem;

#include <hesp/exceptions/Exception.h>
#include <hesp/io/files/DefinitionsFile.h>
#include <hesp/io/files/DefinitionsSpecifierFile.h>
#include <hesp/io/files/NavFile.h>
#include <hesp/io/files/OnionTreeFile.h>
#include <hesp/io/util/DirectoryFinder.h>
#include <hesp/level/bounds/Bounds.h>
#include <hesp/level/bounds/BoundsManager.h>
#include <hesp/level/nav/AdjacencyList.h>
#include <hesp/level/nav/AdjacencyTable.h>
#include <hesp/level/nav/NavDataset.h>
#include <hesp/level/nav/NavManager.h>
#include <hesp/level/nav/NavMeshGenerator.h>
#include <hesp/level/nav/PathTableGenerator.h>
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
	std::cout << "Usage: hnav <input definitions specifier> <input onion tree> <output nav file>" << std::endl;
	exit(EXIT_FAILURE);
}

void run(const std::string& definitionsSpecifierFilename, const std::string& treeFilename, const std::string& outputFilename)
{
	typedef std::vector<CollisionPolygon_Ptr> ColPolyVector;

	// Read in the definitions specifier.
	std::string definitionsFilename = DefinitionsSpecifierFile::load(definitionsSpecifierFilename);

	// Set the appropriate resources directory.
	// FIXME: The game to use shouldn't be hard-coded like this.
	DirectoryFinder& finder = DirectoryFinder::instance();
	finder.set_resources_directory(finder.determine_resources_directory_from_tool("ScarletPimpernel"));

	// Read in the bounds.
	bf::path definitionsDir = finder.determine_definitions_directory();
	BoundsManager_Ptr boundsManager = DefinitionsFile::load_bounds_only((definitionsDir / definitionsFilename).file_string());

	// Read in the polygons and onion tree.
	ColPolyVector polygons;
	OnionTree_Ptr tree;
	OnionTreeFile::load(treeFilename, polygons, tree);

	// Check that the number of bounds and the number of maps in the tree match up.
	int boundsCount = boundsManager->bounds_count();
	int mapCount = tree->map_count();
	if(boundsCount != mapCount) throw Exception("There must be exactly one bounds per map in the onion tree");

	NavManager_Ptr navManager(new NavManager);

	// For each separate map.
	for(int i=0; i<mapCount; ++i)
	{
		// Skip this map if the bounds for it has its nav flag set to false.
		if(!boundsManager->nav_flags()[i]) continue;

		// Make a copy of the polygon array in which all the polygons that aren't
		// in this map are set to non-walkable.
		int polyCount = static_cast<int>(polygons.size());
		ColPolyVector mapPolygons(polyCount);
		for(int j=0; j<polyCount; ++j)
		{
			mapPolygons[j].reset(new CollisionPolygon(*polygons[j]));
			if(mapPolygons[j]->auxiliary_data().map_index() != i)
				mapPolygons[j]->auxiliary_data().set_walkable(false);
		}

		// Generate the navigation mesh.
		double maxHeightDifference = boundsManager->bounds(i)->height() / 2;
		NavMeshGenerator generator(mapPolygons, maxHeightDifference);
		NavMesh_Ptr mesh = generator.generate_mesh();

		// Build the navigation graph adjacency list.
		AdjacencyList_Ptr adjList(new AdjacencyList(mesh));

		// Build the navigation graph adjacency table (note that this is a very inefficient
		// representation for the sparse graph in terms of space, but it's needed for the
		// Floyd-Warshall algorithm used when building the path table).
		AdjacencyTable adjTable(*adjList);

		// Generate the path table.
		PathTable_Ptr pathTable = PathTableGenerator::floyd_warshall(adjTable);

		navManager->set_dataset(i, NavDataset_Ptr(new NavDataset(adjList, mesh, pathTable)));
	}

	// Write the navigation datasets to disk.
	NavFile::save(outputFilename, navManager);
}

int main(int argc, char *argv[])
try
{
	if(argc != 4) quit_with_usage();
	std::vector<std::string> args(argv, argv + argc);
	run(args[1], args[2], args[3]);
	return 0;
}
catch(Exception& e) { quit_with_error(e.cause()); }
