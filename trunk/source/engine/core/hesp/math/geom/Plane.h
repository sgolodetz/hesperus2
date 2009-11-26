/***
 * hesperus: Plane.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_PLANE
#define H_HESP_PLANE

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <hesp/math/vectors/Vector3.h>

namespace hesp {

/**
This enumeration represents the result of classifying an entity against a plane.
*/
enum PlaneClassifier
{
	CP_BACK,
	CP_COPLANAR,
	CP_FRONT,
	CP_STRADDLE		// entities with extent (e.g. polygons) only
};

/**
This class represents planes of the form ax + by + cz - d = 0, i.e. n . x - d = 0.

Datatype invariant: |n| = 1
*/
class Plane
{
	//#################### PRIVATE VARIABLES ####################
private:
	Vector3d m_n;
	double m_d;

	//#################### CONSTRUCTORS ####################
public:
	Plane(const Vector3d& n, double d);
	Plane(const Vector3d& n, const Vector3d& x);

	//#################### PUBLIC METHODS ####################
public:
	double distance_value() const;
	Plane flip() const;
	const Vector3d& normal() const;
	Plane to_undirected_form() const;

	//#################### PRIVATE METHODS ####################
private:
	void ensure_invariant();
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Plane> Plane_Ptr;
typedef shared_ptr<const Plane> Plane_CPtr;

//#################### GLOBAL OPERATORS ####################
std::ostream& operator<<(std::ostream& os, const Plane& plane);
std::ostream& operator<<(std::ostream& os, PlaneClassifier rhs);

}

#endif
