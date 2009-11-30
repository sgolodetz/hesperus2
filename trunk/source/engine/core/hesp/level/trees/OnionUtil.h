/***
 * hesperus: OnionUtil.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ONIONUTIL
#define H_HESP_ONIONUTIL

#include <hesp/math/vectors/Vector3.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<const class OnionNode> OnionNode_CPtr;
typedef shared_ptr<const class OnionTree> OnionTree_CPtr;
typedef shared_ptr<const class Plane> Plane_CPtr;

class OnionUtil
{
	//#################### ENUMERATIONS ####################
public:
	enum RayClassifier
	{
		RAY_EMPTY,				// the ray only lies in leaves of the tree which are empty in the relevant map
		RAY_SOLID,				// the ray only lies in leaves of the tree which are solid in the relevant map
		RAY_TRANSITION_ES,		// the first transition of the ray in the relevant map is from an empty leaf to a solid one
		RAY_TRANSITION_SE		// the first transition of the ray in the relevant map is from a solid leaf to an empty one
	};

	//#################### NESTED CLASSES ####################
public:
	struct Transition
	{
		RayClassifier classifier;
		Vector3d_Ptr location;
		Plane_CPtr plane;

		explicit Transition(RayClassifier classifier_, const Vector3d_Ptr& location_ = Vector3d_Ptr(), const Plane_CPtr& plane_ = Plane_CPtr())
		:	classifier(classifier_), location(location_), plane(plane_)
		{}
	};

	typedef shared_ptr<Transition> Transition_Ptr;
	typedef shared_ptr<const Transition> Transition_CPtr;

	//#################### PUBLIC METHODS ####################
public:
	static Transition find_first_transition(int mapIndex, const Vector3d& source, const Vector3d& dest, const OnionTree_CPtr& tree);

	//#################### PRIVATE METHODS ####################
private:
	static Transition find_first_transition_sub(int mapIndex, const Vector3d& source, const Vector3d& dest, const OnionNode_CPtr& node);
};

}

#endif
