/***
 * hesperus: Animation.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ANIMATION
#define H_HESP_ANIMATION

#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<const class Pose> Pose_CPtr;

class Animation
{
	//#################### PRIVATE VARIABLES ####################
private:
	double m_length;
	std::vector<Pose_CPtr> m_keyframes;

	//#################### CONSTRUCTORS ####################
public:
	Animation(double length, const std::vector<Pose_CPtr>& keyframes);

	//#################### PUBLIC METHODS ####################
public:
	int keyframe_count() const;
	Pose_CPtr keyframe(int i) const;
	double length() const;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Animation> Animation_Ptr;
typedef shared_ptr<const Animation> Animation_CPtr;

}

#endif
