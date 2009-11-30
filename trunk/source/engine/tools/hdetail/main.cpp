/***
 * hdetail: main.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <iostream>
#include <list>
#include <string>
#include <vector>

#include <hesp/exceptions/Exception.h>
#include <hesp/io/files/BrushesFile.h>
#include <hesp/io/files/TreeFile.h>
#include <hesp/level/brushes/PolyhedralBrush.h>
#include <hesp/level/csg/CSGUtil.h>
#include <hesp/level/trees/BSPUtil.h>
#include <hesp/util/PolygonTypes.h>
using namespace hesp;

//#################### TYPEDEFS ####################
typedef std::list<TexturedPolygon_Ptr> TexPolyList;
typedef shared_ptr<TexPolyList> TexPolyList_Ptr;
typedef std::vector<TexturedPolygon_Ptr> TexPolyVector;
typedef PolyhedralBrush<TexturedPolygon> TexPolyBrush;
typedef shared_ptr<TexPolyBrush> TexPolyBrush_Ptr;
typedef std::vector<TexPolyBrush_Ptr> TexPolyBrushVector;

//#################### FUNCTIONS ####################
void quit_with_error(const std::string& error)
{
	std::cout << "Error: " << error << std::endl;
	exit(EXIT_FAILURE);
}

void quit_with_usage()
{
	std::cout << "Usage: hdetail <input BSP tree> <input detail geometry> <output BSP tree>" << std::endl;
	exit(EXIT_FAILURE);
}

void run_detailer(const std::string& inputBSPFilename, const std::string& inputDetailGeometryFilename,
				  const std::string& outputBSPFilename)
{
	typedef TexturedPolygon::Vert Vert;
	typedef TexturedPolygon::AuxData AuxData;

	// Read in the polygons and tree.
	TexPolyVector polygons;
	BSPTree_Ptr tree;
	TreeFile::load(inputBSPFilename, polygons, tree);

	// Read in the detail brushes and perform a CSG union on them.
	TexPolyBrushVector detailBrushes = BrushesFile::load<TexturedPolygon>(inputDetailGeometryFilename);
	TexPolyList_Ptr detailFaces = CSGUtil<Vert,AuxData>::union_all(detailBrushes);

	// Clip the detail faces to the tree.
	TexPolyList fragments = CSGUtil<Vert,AuxData>::clip_polygons_to_tree(*detailFaces, tree, true);

	// Add the face fragments to the polygons array.
	int firstFragment = static_cast<int>(polygons.size());
	std::copy(fragments.begin(), fragments.end(), std::back_inserter(polygons));
	int lastFragment = static_cast<int>(polygons.size()) - 1;

	// Add the face fragments to the relevant leaves.
	for(int i=firstFragment; i<=lastFragment; ++i)
	{
		std::list<int> leafIndices = BSPUtil::find_leaf_indices(*polygons[i], tree);
		for(std::list<int>::const_iterator jt=leafIndices.begin(), jend=leafIndices.end(); jt!=jend; ++jt)
		{
			BSPLeaf *leaf = tree->leaf(*jt);
			leaf->add_polygon_index(i);
		}
	}

	// Write the modified polygon array and tree to disk.
	TreeFile::save(outputBSPFilename, polygons, tree);
}

int main(int argc, char *argv[])
try
{
	if(argc != 4) quit_with_usage();
	std::vector<std::string> args(argv, argv + argc);
	run_detailer(args[1], args[2], args[3]);
	return 0;
}
catch(Exception& e) { quit_with_error(e.cause()); }
