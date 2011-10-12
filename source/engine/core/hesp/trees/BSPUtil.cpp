/***
 * hesperus: BSPUtil.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "BSPUtil.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
/**
Determines whether or not there is line-of-sight between p1 and p2 in the specified BSP tree.

@param p1		A point in the world
@param p2		Another point in the world
@param tree		The BSP tree
@return			true, if there is line-of-sight between the points, or false otherwise
*/
bool BSPUtil::line_of_sight(const Vector3d& p1, const Vector3d& p2, const BSPTree_CPtr& tree)
{
	return line_of_sight_sub(p1, p2, tree->root());
}

//#################### PRIVATE METHODS ####################
bool BSPUtil::line_of_sight_sub(const Vector3d& p1, const Vector3d& p2, const BSPNode_CPtr& node)
{
	if(node->is_leaf())
	{
		// If both points are in the same leaf, there's line-of-sight provided the leaf's not solid.
		const BSPLeaf *leaf = node->as_leaf();
		return !leaf->is_solid();
	}

	const BSPBranch *branch = node->as_branch();
	PlaneClassifier cp1, cp2;
	switch(classify_linesegment_against_plane(p1, p2, *branch->splitter(), cp1, cp2))
	{
		case CP_BACK:
		{
			return line_of_sight_sub(p1, p2, branch->right());
		}
		case CP_COPLANAR:
		case CP_FRONT:
		{
			return line_of_sight_sub(p1, p2, branch->left());
		}
		default:	// case CP_STRADDLE
		{
			Vector3d q = determine_linesegment_intersection_with_plane(p1, p2, *branch->splitter()).first;
			if(cp1 == CP_BACK)
			{
				// cp2 == CP_FRONT
				return line_of_sight_sub(p1, q, branch->right()) && line_of_sight_sub(q, p2, branch->left());
			}
			else
			{
				// cp1 == CP_FRONT, cp2 == CP_BACK
				return line_of_sight_sub(p1, q, branch->left()) && line_of_sight_sub(q, p2, branch->right());
			}
		}
	}
}

}
