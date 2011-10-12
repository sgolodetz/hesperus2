/***
 * hesperus: OnionTreeFile.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <fstream>

#include <hesp/exceptions/Exception.h>
#include <hesp/io/sections/OnionTreeSection.h>
#include <hesp/io/sections/PolygonsSection.h>

namespace hesp {

//#################### LOADING METHODS ####################
/**
Loads the polygons and onion tree from the specified onion tree file.

@param filename		The name of the onion tree file
@param polygons		Used to return the polygons to the caller
@param tree			Used to return the onion tree to the caller
*/
template <typename Poly>
void OnionTreeFile::load(const std::string& filename, std::vector<shared_ptr<Poly> >& polygons, OnionTree_Ptr& tree)
{
	std::ifstream is(filename.c_str(), std::ios_base::binary);
	if(is.fail()) throw Exception("Could not open " + filename + " for reading");

	PolygonsSection::load(is, "Polygons", polygons);
	tree = OnionTreeSection::load(is);
}

//#################### SAVING METHODS ####################
/**
Saves the polygons and onion tree to the specified onion tree file.

@param filename		The name of the onion tree file
@param polygons		The polygons
@param tree			The onion tree
*/
template <typename Poly>
void OnionTreeFile::save(const std::string& filename, const std::vector<shared_ptr<Poly> >& polygons, const OnionTree_CPtr& tree)
{
	std::ofstream os(filename.c_str(), std::ios_base::binary);
	if(os.fail()) throw Exception("Could not open " + filename + " for writing");

	PolygonsSection::save(os, "Polygons", polygons);
	OnionTreeSection::save(os, tree);
}

}
