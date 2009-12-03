/***
 * hesperus: VectorUtil.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "VectorUtil.h"

#include <cmath>

#include <hesp/exceptions/Exception.h>
#include <hesp/math/Constants.h>

namespace hesp {

Vector3d rotate_about_axis(const Vector3d& v, double degreeAngle, const Vector3d& axis)
{
	if(fabs(axis.length_squared() - 1) > EPSILON) throw Exception("The axis of rotation must be normalized");

	double radianAngle = degreeAngle*PI/180.0;
	double cosAngle = cos(radianAngle), sinAngle = sin(radianAngle);
	Vector3d aCROSSv = axis.cross(v);

	// ret = v cos radianAngle + (axis x v) sin radianAngle + axis(axis . v)(1 - cos radianAngle)
	// (See Mathematics for 3D Game Programming and Computer Graphics, P.62, for details of why this is (it's not very hard)).
	Vector3d ret = cosAngle*v;
	ret += sinAngle*aCROSSv;
	ret += (axis.dot(v) * (1 - cosAngle)) * axis;
	return ret;
}

}
