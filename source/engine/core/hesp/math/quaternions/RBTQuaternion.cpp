/***
 * hesperus: RBTQuaternion.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "RBTQuaternion.h"

#include <cmath>

#include <hesp/exceptions/Exception.h>
#include <hesp/math/Constants.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
RBTQuaternion::RBTQuaternion(const Quaternion& rotation, const Vector3d& translation)
:	m_rotation(rotation), m_translation(translation)
{
	if(fabs(m_rotation.length_squared() - 1) > SMALL_EPSILON)
	{
		throw Exception("RBTQuaternion: The quaternion rotation component must be of unit length");
	}
}

//#################### STATIC FACTORY METHODS ####################
RBTQuaternion_Ptr RBTQuaternion::from_axis_angle_translation(Vector3d axis, double angle, const Vector3d& translation)
{
	return RBTQuaternion_Ptr(new RBTQuaternion(Quaternion::from_axis_angle(axis, angle), translation));
}

//#################### PUBLIC METHODS ####################
Vector3d RBTQuaternion::apply(const Vector3d& p) const
{
	return m_rotation.apply_unit_rotation(p) + m_translation;
}

RBTQuaternion_Ptr RBTQuaternion::interpolate(const RBTQuaternion_CPtr& lhs, const RBTQuaternion_CPtr& rhs, double t)
{
	Quaternion newRot = Quaternion::slerp(lhs->rotation(), rhs->rotation(), t);
	Vector3d newTrans = (1-t)*lhs->translation() + t*rhs->translation();
	return RBTQuaternion_Ptr(new RBTQuaternion(newRot, newTrans));
}

RBTQuaternion_Ptr RBTQuaternion::inverse() const
{
	// v' = R*v + T
	// => v = inv(R)*(v' - T) = inv(R)*v' + (-inv(R)*T)
	Quaternion invRot = m_rotation.inverse();
	Vector3d invTrans = -invRot.apply_unit_rotation(m_translation);
	return RBTQuaternion_Ptr(new RBTQuaternion(invRot, invTrans));
}

const Quaternion& RBTQuaternion::rotation() const
{
	return m_rotation;
}

const Vector3d& RBTQuaternion::translation() const
{
	return m_translation;
}

}
