/***
 * hesperus: FixedCamera.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "FixedCamera.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
FixedCamera::FixedCamera(const Vector3d& eye, const Vector3d& look, const Vector3d& up)
:	m_eye(eye), m_at(eye + look), m_up(up)
{}

//#################### PUBLIC METHODS ####################
Vector3d FixedCamera::at() const
{
	return m_at;
}

Vector3d FixedCamera::eye() const
{
	return m_eye;
}

bool FixedCamera::is_inside_player() const
{
	return false;
}

Vector3d FixedCamera::look() const
{
	return m_at - m_eye;
}

Vector3d FixedCamera::up() const
{
	return m_up;
}

void FixedCamera::update()
{
	// No-op
}

}
