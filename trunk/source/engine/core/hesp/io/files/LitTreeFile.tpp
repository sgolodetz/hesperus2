/***
 * hesperus: LitTreeFile.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <fstream>

#include <hesp/io/sections/LightmapPrefixSection.h>
#include <hesp/io/sections/PolygonsSection.h>
#include <hesp/io/sections/TreeSection.h>

namespace hesp {

//#################### LOADING METHODS ####################
/**
Loads the polygons, tree and lightmap prefix from the specified lit tree file.

@param filename			The name of the lit tree file
@param polygons			Used to return the polygons to the caller
@param tree				Used to return the tree to the caller
@param lightmapPrefix	Used to return the lightmap prefix to the caller
*/
template <typename Poly>
void LitTreeFile::load(const std::string& filename, std::vector<shared_ptr<Poly> >& polygons, BSPTree_Ptr& tree,
					   std::string& lightmapPrefix)
{
	std::ifstream is(filename.c_str(), std::ios_base::binary);
	if(is.fail()) throw Exception("Could not open " + filename + " for reading");

	PolygonsSection::load(is, "Polygons", polygons);
	tree = TreeSection::load(is);
	lightmapPrefix = LightmapPrefixSection::load(is);
}

//#################### SAVING METHODS ####################
/**
Saves the polygons, tree and lightmap prefix to the specified lit tree file.

@param filename			The name of the lit tree file
@param polygons			The polygons
@param tree				The tree
@param lightmapPrefix	The lightmap prefix
*/
template <typename Poly>
void LitTreeFile::save(const std::string& filename, const std::vector<shared_ptr<Poly> >& polygons,
					   const BSPTree_CPtr& tree, const std::string& lightmapPrefix)
{
	std::ofstream os(filename.c_str(), std::ios_base::binary);
	if(os.fail()) throw Exception("Could not open " + filename + " for writing");

	PolygonsSection::save(os, "Polygons", polygons);
	TreeSection::save(os, tree);
	LightmapPrefixSection::save(os, lightmapPrefix);
}

}
