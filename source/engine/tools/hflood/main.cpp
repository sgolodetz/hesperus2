/***
 * hflood: main.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include <hesp/io/files/GeometryFile.h>
#include <hesp/io/files/PortalsFile.h>
#include <hesp/io/files/TreeFile.h>
#include <hesp/trees/TreeUtil.h>
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
	std::cout << "Usage: hflood {-r|-c} <input tree> <input portals> <output geometry>" << std::endl;
	exit(EXIT_FAILURE);
}

void flood_from(int leaf, const std::map<int,std::vector<Portal_Ptr> >& portalsFromLeaf, std::set<int>& reachableLeaves)
{
	reachableLeaves.insert(leaf);

	std::map<int,std::vector<Portal_Ptr> >::const_iterator it = portalsFromLeaf.find(leaf);
	if(it != portalsFromLeaf.end())
	{
		const std::vector<Portal_Ptr>& outPortals = it->second;
		int outPortalCount = static_cast<int>(outPortals.size());
		for(int j=0; j<outPortalCount; ++j)
		{
			Portal_Ptr outPortal = outPortals[j];
			int toLeaf = outPortal->auxiliary_data().toLeaf;

			// If the destination of this portal is already marked as reachable, don't recurse.
			if(reachableLeaves.find(toLeaf) != reachableLeaves.end()) continue;

			// Otherwise, recursively flood from the leaf on the other side of this portal.
			flood_from(toLeaf, portalsFromLeaf, reachableLeaves);
		}
	}
}

template <typename Poly>
void run_flood(const std::string& treeFilename, const std::string& portalsFilename, const std::string& outputFilename)
{
	// Load the polygons and tree.
	typedef shared_ptr<Poly> Poly_Ptr;
	typedef std::vector<Poly_Ptr> PolyVector;
	PolyVector polygons;
	BSPTree_Ptr tree;
	TreeFile::load(treeFilename, polygons, tree);

	// Load the portals.
	int emptyLeafCount;
	std::vector<Portal_Ptr> portals;
	PortalsFile::load(portalsFilename, emptyLeafCount, portals);

	// Build the "portals from leaf" data structure.
	std::map<int,std::vector<Portal_Ptr> > portalsFromLeaf;
	for(std::vector<Portal_Ptr>::const_iterator it=portals.begin(), iend=portals.end(); it!=iend; ++it)
	{
		int fromLeaf = (*it)->auxiliary_data().fromLeaf;
		portalsFromLeaf[fromLeaf].push_back(*it);
	}

	// Flood from an arbitrary point outside the level to figure out which leaves aren't valid.
	int startLeaf = TreeUtil::find_leaf_index(Vector3d(100000, 0, 0), tree);

	std::set<int> reachableLeaves;
	flood_from(startLeaf, portalsFromLeaf, reachableLeaves);

	// Determine the set of valid leaves, i.e. the ones which aren't reachable from outside the level.
	std::set<int> emptyLeaves;
	for(int i=0; i<emptyLeafCount; ++i) emptyLeaves.insert(i);
	std::set<int> validLeaves;
	std::set_difference(emptyLeaves.begin(), emptyLeaves.end(), reachableLeaves.begin(), reachableLeaves.end(), std::inserter(validLeaves, validLeaves.end()));

	// Copy all the polygons from them to an array.
	PolyVector validPolygons;
	validPolygons.reserve(polygons.size());
	for(std::set<int>::const_iterator it=validLeaves.begin(), iend=validLeaves.end(); it!=iend; ++it)
	{
		const BSPLeaf *leaf = tree->leaf(*it);
		const std::vector<int>& polyIndices = leaf->polygon_indices();
		for(std::vector<int>::const_iterator jt=polyIndices.begin(), jend=polyIndices.end(); jt!=jend; ++jt)
		{
			validPolygons.push_back(polygons[*jt]);
		}
	}

	// Write the polygons to the output file.
	GeometryFile::save(outputFilename, validPolygons);
}

int main(int argc, char *argv[])
try
{
	if(argc != 5) quit_with_usage();
	std::vector<std::string> args(argv, argv + argc);

	if(args[1] == "-r") run_flood<TexturedPolygon>(args[2], args[3], args[4]);
	else if(args[1] == "-c") run_flood<CollisionPolygon>(args[2], args[3], args[4]);
	else quit_with_usage();

	return 0;
}
catch(Exception& e) { quit_with_error(e.cause()); }
