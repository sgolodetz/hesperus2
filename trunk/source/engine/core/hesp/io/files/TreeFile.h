/***
 * hesperus: TreeFile.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_TREEFILE
#define H_HESP_TREEFILE

#include <hesp/level/trees/BSPTree.h>

namespace hesp {

struct TreeFile
{
	//#################### LOADING METHODS ####################
	template <typename Poly> static void load(const std::string& filename, std::vector<shared_ptr<Poly> >& polygons, BSPTree_Ptr& tree);

	//#################### SAVING METHODS ####################
	template <typename Poly> static void save(const std::string& filename, const std::vector<shared_ptr<Poly> >& polygons, const BSPTree_CPtr& tree);
};

}

#include "TreeFile.tpp"

#endif
