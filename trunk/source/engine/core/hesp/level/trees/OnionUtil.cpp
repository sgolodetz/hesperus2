/***
 * hesperus: OnionUtil.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "OnionUtil.h"

#include <boost/lexical_cast.hpp>
using boost::lexical_cast;

#include <hesp/exceptions/Exception.h>
#include <hesp/math/geom/GeomUtil.h>
#include "OnionTree.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
OnionUtil::Transition
OnionUtil::find_first_transition(int mapIndex, const Vector3d& source, const Vector3d& dest, const OnionTree_CPtr& tree)
{
	if(0 <= mapIndex && mapIndex < tree->map_count())
	{
		return find_first_transition_sub(mapIndex, source, dest, tree->root());
	}
	else throw Exception("The onion tree does not contain a map with index " + lexical_cast<std::string>(mapIndex));
}

//#################### PRIVATE METHODS ####################
OnionUtil::Transition
OnionUtil::find_first_transition_sub(int mapIndex, const Vector3d& source, const Vector3d& dest, const OnionNode_CPtr& node)
{
	if(node->is_leaf())
	{
		const OnionLeaf *leaf = node->as_leaf();
		if(leaf->is_solid(mapIndex)) return Transition(RAY_SOLID);
		else return Transition(RAY_EMPTY);
	}

	const OnionBranch *branch = node->as_branch();
	OnionNode_CPtr left = branch->left();
	OnionNode_CPtr right = branch->right();

	Plane_CPtr splitter = branch->splitter();
	PlaneClassifier cpSource, cpDest;
	switch(classify_linesegment_against_plane(source, dest, *splitter, cpSource, cpDest))
	{
		case CP_BACK:
		{
			return find_first_transition_sub(mapIndex, source, dest, right);
		}
		case CP_COPLANAR:
		{
			Transition trLeft = find_first_transition_sub(mapIndex, source, dest, left);
			Transition trRight = find_first_transition_sub(mapIndex, source, dest, right);
			if(trLeft.classifier == trRight.classifier)
			{
				switch(trLeft.classifier)
				{
					case RAY_EMPTY:
					case RAY_SOLID:
					{
						return trLeft;
					}
					default:
					{
						double dLeft = source.distance_squared(*trLeft.location);
						double dRight = source.distance_squared(*trRight.location);
						if(dLeft < dRight) return trLeft;
						else return trRight;
					}
				}
			}
			else if(trLeft.classifier == RAY_TRANSITION_ES || trLeft.classifier == RAY_TRANSITION_SE)
			{
				return trLeft;
			}
			else if(trRight.classifier == RAY_TRANSITION_ES || trRight.classifier == RAY_TRANSITION_SE)
			{
				return trRight;
			}
			else
			{
				return Transition(RAY_EMPTY);
			}
		}
		case CP_FRONT:
		{
			return find_first_transition_sub(mapIndex, source, dest, left);
		}
		default:	// case CP_STRADDLE
		{
			Vector3d mid = determine_linesegment_intersection_with_plane(source, dest, *splitter).first;
			OnionNode_CPtr near, far;
			if(cpSource == CP_FRONT)
			{
				near = left;
				far = right;
			}
			else
			{
				near = right;
				far = left;
			}

			// Search for a transition on the near side of the plane: if we find one, that's the first transition.
			Transition trNear = find_first_transition_sub(mapIndex, source, mid, near);
			if(trNear.location) return trNear;

			// Search for a transition on the far side of the plane.
			Transition trFar = find_first_transition_sub(mapIndex, mid, dest, far);

			switch(trFar.classifier)
			{
				case RAY_EMPTY:
				{
					// If both sides are empty, there's no transition, otherwise there's a solid -> empty transition
					// at the point where the ray intersects the current split plane.
					if(trNear.classifier == RAY_EMPTY) return Transition(RAY_EMPTY);
					else return Transition(RAY_TRANSITION_SE, Vector3d_Ptr(new Vector3d(mid)), splitter);
				}
				case RAY_SOLID:
				{
					// If both sides are solid, there's no transition, otherwise there's an empty -> solid transition
					// at the point where the ray intersects the current split plane.
					if(trNear.classifier == RAY_EMPTY) return Transition(RAY_TRANSITION_ES, Vector3d_Ptr(new Vector3d(mid)), splitter);
					else return Transition(RAY_SOLID);
				}
				case RAY_TRANSITION_ES:
				{
					// If the near side is empty, the first transition is the empty -> solid one on the far side.
					// Otherwise, there's a nearer solid -> empty transition on the current split plane.
					if(trNear.classifier == RAY_EMPTY) return trFar;
					else return Transition(RAY_TRANSITION_SE, Vector3d_Ptr(new Vector3d(mid)), splitter);
				}
				default:	// case RAY_TRANSITION_SE
				{
					// If the near side is solid, the first transition is the solid -> empty one on the far side.
					// Otherwise, there's a nearer empty -> solid transition on the current split plane.
					if(trNear.classifier == RAY_SOLID) return trFar;
					else return Transition(RAY_TRANSITION_ES, Vector3d_Ptr(new Vector3d(mid)), splitter);
				}
			}
		}
	}
}

}
