/***
 * hesperus: Sphere.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Sphere.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
Sphere::Sphere(const Vector3d& centre, double radius)
:	m_centre(centre), m_radius(radius)
{}

//#################### PUBLIC METHODS ####################
const Vector3d& Sphere::centre() const
{
	return m_centre;
}

double Sphere::radius() const
{
	return m_radius;
}

}
