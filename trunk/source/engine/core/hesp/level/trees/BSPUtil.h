/***
 * hesperus: BSPUtil.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_BSPUTIL
#define H_HESP_BSPUTIL

#include <list>

#include <hesp/math/geom/Polygon.h>
#include <hesp/math/vectors/Vector3.h>
#include "BSPTree.h"

namespace hesp {

class BSPUtil
{
	//#################### PUBLIC METHODS ####################
public:
	template <typename Vert, typename AuxData> static std::list<int> find_leaf_indices(const Polygon<Vert,AuxData>& poly, const BSPTree_CPtr& tree);
	static bool line_of_sight(const Vector3d& p1, const Vector3d& p2, const BSPTree_CPtr& tree);

	//#################### PRIVATE METHODS ####################
private:
	template <typename Vert, typename AuxData> static std::list<int> find_leaf_indices_sub(const Polygon<Vert,AuxData>& poly, const BSPNode_CPtr& node);
	static bool line_of_sight_sub(const Vector3d& p1, const Vector3d& p2, const BSPNode_CPtr& node);
};

}

#include "BSPUtil.tpp"

#endif
