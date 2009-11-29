/***
 * hesperus: TreeUtil.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_TREEUTIL
#define H_HESP_TREEUTIL

#include <list>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <hesp/math/geom/Plane.h>
#include <hesp/math/vectors/Vector3.h>

namespace hesp {

class TreeUtil
{
	//#################### PUBLIC METHODS ####################
public:
	template <typename Tree> static int find_leaf_index(const Vector3d& p, shared_ptr<Tree> tree);
	template <typename Tree> static int find_leaf_index(const Vector3d& p, shared_ptr<const Tree> tree);
	template <typename Tree> static std::list<Plane_CPtr> split_planes(shared_ptr<Tree> tree);
	template <typename Tree> static std::list<Plane_CPtr> split_planes(shared_ptr<const Tree> tree);

	//#################### PRIVATE METHODS ####################
private:
	template <typename Tree> static std::list<Plane_CPtr> split_planes_sub(shared_ptr<const typename Tree::Node> node);
};

}

#include "TreeUtil.tpp"

#endif
