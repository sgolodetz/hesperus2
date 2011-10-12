/***
 * hesperus: VerticallyConstrainedNUVAxes.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "VerticallyConstrainedNUVAxes.h"

#include <hesp/math/Constants.h>
#include <hesp/math/vectors/VectorUtil.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
VerticallyConstrainedNUVAxes::VerticallyConstrainedNUVAxes(const Vector3d& n)
:	NUVAxes(n, Vector3d(0,0,1))
{}

//#################### PUBLIC METHODS ####################
void VerticallyConstrainedNUVAxes::rotate(const Vector3d& axis, double angle)
{
	// Note 1:	We try and optimise things a little by observing that there's no point rotating an
	//			axis about itself and that generally when we rotate about an axis, we'll be passing
	//			it in as the parameter axis, e.g. rotate(n(), Math.PI/2).

	// Note 2:	We prevent the new n vector from being directly upwards or downwards.

	// Note 3:	We prevent the new v vector from pointing below the horizontal.

	Vector3d newN = m_n;
	if(&axis != &m_n)
	{
		newN = rotate_about_axis(m_n, angle, axis);
		if(newN.cross(Vector3d(0,0,1)).length() < EPSILON) return;
	}

	Vector3d newV = m_v;
	if(&axis != &m_v)
	{
		newV = rotate_about_axis(m_v, angle, axis);
		if(newV.z < 0) return;
	}

	m_n = newN;
	m_v = newV;

	if(&axis != &m_u) m_u = rotate_about_axis(m_u, angle, axis);
}

void VerticallyConstrainedNUVAxes::set_n(const Vector3d& n)
{
	NUVAxes::set_n(n, Vector3d(0,0,1));
}

}
