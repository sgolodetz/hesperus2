/***
 * hesperus: NavMeshUtil.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_NAVMESHUTIL
#define H_HESP_NAVMESHUTIL

#include <hesp/util/PolygonTypes.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<const class NavMesh> NavMesh_CPtr;
typedef shared_ptr<const class OnionTree> OnionTree_CPtr;

struct NavMeshUtil
{
	//#################### PUBLIC METHODS ####################
	static int find_nav_polygon(const Vector3d& p, int suggestedNavPoly, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_CPtr& tree, const NavMesh_CPtr& navMesh);
};

}

#endif
