/***
 * hesperus: UniquePlanePred.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "UniquePlanePred.h"

#include <cmath>

#include "Plane.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
UniquePlanePred::UniquePlanePred(double angleTolerance, double distTolerance)
:	m_angleTolerance(fabs(angleTolerance)), m_distTolerance(fabs(distTolerance))
{}

//#################### PUBLIC OPERATORS ####################
bool UniquePlanePred::operator()(const Plane& lhs, const Plane& rhs) const
{
	// If these planes are nearly the same (in terms of normal direction
	// and distance value), then !(lhs < rhs) && !(rhs < lhs).
	double dotProd = lhs.normal().dot(rhs.normal());
	if(dotProd > 1.0) dotProd = 1.0;	// acos(x) is only defined for x <= 1, so clamp dotProd to avoid floating-point problems
	double angle = acos(dotProd);
	double dist = lhs.distance_value() - rhs.distance_value();
	if(fabs(angle) < m_angleTolerance && fabs(dist) < m_distTolerance) return false;

	// Otherwise, compare the two planes "lexicographically".
	const Vector3d& nL = lhs.normal(), nR = rhs.normal();
	const double& aL = nL.x, bL = nL.y, cL = nL.z;
	const double& aR = nR.x, bR = nR.y, cR = nR.z;
	const double& dL = lhs.distance_value(), dR = rhs.distance_value();

	return ((aL < aR) ||
			(aL == aR && bL < bR) ||
			(aL == aR && bL == bR && cL < cR) ||
			(aL == aR && bL == bR && cL == cR && dL < dR));
}

}
