/***
 * hesperus: PoseModifier.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_POSEMODIFIER
#define H_HESP_POSEMODIFIER

#include <hesp/math/vectors/Vector3.h>

namespace hesp {

struct PoseModifier
{
	Vector3d axis;
	double angle;

	PoseModifier(const Vector3d& axis_, double angle_)
	:	axis(axis_), angle(angle_)
	{}
};

}

#endif
