/***
 * hesperus: UniquePlanePred.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_UNIQUEPLANEPRED
#define H_HESP_UNIQUEPLANEPRED

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
class Plane;

struct UniquePlanePred
{
	//#################### PUBLIC VARIABLES ####################
	double m_angleTolerance, m_distTolerance;

	//#################### CONSTRUCTORS ####################
	UniquePlanePred(double angleTolerance, double distTolerance);

	//#################### PUBLIC OPERATORS ####################
	bool operator()(const Plane& lhs, const Plane& rhs) const;
};

}

#endif
