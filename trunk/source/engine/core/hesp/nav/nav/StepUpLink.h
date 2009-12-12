/***
 * hesperus: StepUpLink.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_STEPUPLINK
#define H_HESP_STEPUPLINK

#include "StepLink.h"

namespace hesp {

class StepUpLink : public StepLink
{
	//#################### CONSTRUCTORS ####################
public:
	StepUpLink(int sourcePoly, int destPoly, const Vector3d& s1, const Vector3d& s2, const Vector3d& d1, const Vector3d& d2);
	StepUpLink(int sourcePoly, int destPoly, const LineSegment3d& sourceEdge, const LineSegment3d& destEdge);

	//#################### PUBLIC METHODS ####################
public:
	std::string link_name() const;
	static NavLink_Ptr load(const std::string& data);
	void render() const;
};

}

#endif
