/***
 * hesperus: TreeFile.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <hesp/exceptions/Exception.h>
#include <hesp/io/sections/PolygonsSection.h>
#include <hesp/io/sections/TreeSection.h>

namespace hesp {

//#################### LOADING METHODS ####################
/**
Loads the polygons and BSP tree from the specified tree file.

@param filename		The name of the tree file
@param polygons		Used to return the polygons to the caller
@param tree			Used to return the tree to the caller
*/
template <typename Poly>
void TreeFile::load(const std::string& filename, std::vector<shared_ptr<Poly> >& polygons, BSPTree_Ptr& tree)
{
	std::ifstream is(filename.c_str());
	if(is.fail()) throw Exception("Could not open " + filename + " for reading");

	PolygonsSection::load(is, "Polygons", polygons);
	tree = TreeSection::load(is);
}

//#################### SAVING METHODS ####################
/**
Saves the polygons and BSP tree to the specified tree file.

@param filename		The name of the tree file
@param polygons		The polygons
@param tree			The tree
*/
template <typename Poly>
void TreeFile::save(const std::string& filename, const std::vector<shared_ptr<Poly> >& polygons, const BSPTree_CPtr& tree)
{
	std::ofstream os(filename.c_str());
	if(os.fail()) throw Exception("Could not open " + filename + " for writing");

	PolygonsSection::save(os, "Polygons", polygons);
	TreeSection::save(os, tree);
}

}
