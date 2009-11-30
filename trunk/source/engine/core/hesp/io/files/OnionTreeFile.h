/***
 * hesperus: OnionTreeFile.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ONIONTREEFILE
#define H_HESP_ONIONTREEFILE

#include <hesp/level/trees/OnionTree.h>

namespace hesp {

struct OnionTreeFile
{
	//#################### LOADING METHODS ####################
	template <typename Poly> static void load(const std::string& filename, std::vector<shared_ptr<Poly> >& polygons, OnionTree_Ptr& tree);

	//#################### SAVING METHODS ####################
	template <typename Poly> static void save(const std::string& filename, const std::vector<shared_ptr<Poly> >& polygons, const OnionTree_CPtr& tree);
};

}

#include "OnionTreeFile.tpp"

#endif
