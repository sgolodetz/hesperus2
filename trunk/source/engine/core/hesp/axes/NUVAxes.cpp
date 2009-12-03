/***
 * hesperus: NUVAxes.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "NUVAxes.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
/**
Constructs a set of NUV (i.e. camera) axes.

@param n	The n axis (the direction in which the camera is pointing)
@param up	The world up vector

Preconditions:

- n and up must not be collinear
*/
NUVAxes::NUVAxes(const Vector3d& n, const Vector3d& up)
{
	set_n(n, up);
}

//#################### DESTRUCTOR ####################
NUVAxes::~NUVAxes() {}

//#################### PUBLIC METHODS ####################
const Vector3d& NUVAxes::n() const	{ return m_n; }

void NUVAxes::set_n(const Vector3d& n, const Vector3d& up)
{
	m_n = n;
	m_n.normalize();
	m_u = up.cross(m_n).normalize();
	m_v = m_n.cross(m_u).normalize();
}

const Vector3d& NUVAxes::u() const	{ return m_u; }
const Vector3d& NUVAxes::v() const	{ return m_v; }

}
