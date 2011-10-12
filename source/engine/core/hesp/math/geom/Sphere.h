/***
 * hesperus: Sphere.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_SPHERE
#define H_HESP_SPHERE

#include <hesp/math/vectors/Vector3.h>

namespace hesp {

class Sphere
{
	//#################### PRIVATE VARIABLES ####################
private:
	Vector3d m_centre;
	double m_radius;

	//#################### CONSTRUCTORS ####################
public:
	Sphere(const Vector3d& centre, double radius);

	//#################### PUBLIC METHODS ####################
public:
	const Vector3d& centre() const;
	double radius() const;
};

}

#endif
