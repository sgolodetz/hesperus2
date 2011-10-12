/***
 * hesperus: LitTreeFile.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LITTREEFILE
#define H_HESP_LITTREEFILE

#include <hesp/trees/BSPTree.h>

namespace hesp {

struct LitTreeFile
{
	//#################### LOADING METHODS ####################
	template <typename Poly> static void load(const std::string& filename, std::vector<shared_ptr<Poly> >& polygons,
											  BSPTree_Ptr& tree, std::string& lightmapPrefix);

	//#################### SAVING METHODS ####################
	template <typename Poly> static void save(const std::string& filename, const std::vector<shared_ptr<Poly> >& polygons,
											  const BSPTree_CPtr& tree, const std::string& lightmapPrefix);
};

}

#include "LitTreeFile.tpp"

#endif
