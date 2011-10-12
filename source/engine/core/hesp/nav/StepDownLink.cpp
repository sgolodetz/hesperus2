/***
 * hesperus: StepDownLink.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "StepDownLink.h"

#include <sstream>

#include <hesp/ogl/WrappedGL.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
StepDownLink::StepDownLink(int sourcePoly, int destPoly, const Vector3d& s1, const Vector3d& s2, const Vector3d& d1, const Vector3d& d2)
:	StepLink(sourcePoly, destPoly, s1, s2, d1, d2)
{}

StepDownLink::StepDownLink(int sourcePoly, int destPoly, const LineSegment3d& sourceEdge, const LineSegment3d& destEdge)
:	StepLink(sourcePoly, destPoly, sourceEdge, destEdge)
{}

//#################### PUBLIC METHODS ####################
std::string StepDownLink::link_name() const
{
	return "StepDown";
}

NavLink_Ptr StepDownLink::load(const std::string& data)
{
	std::stringstream ss;
	ss << data;

	std::ios_base::fmtflags oldFlags = ss.flags();
	ss.setf(std::ios_base::skipws);

	int sourcePoly, destPoly;
	LineSegment3d sourceEdge, destEdge;

	ss >> sourcePoly >> destPoly >> sourceEdge >> destEdge;

	ss.flags(oldFlags);

	return NavLink_Ptr(new StepDownLink(sourcePoly, destPoly, sourceEdge, destEdge));
}

void StepDownLink::render() const
{
	Vector3d s = (m_sourceEdge.e1 + m_sourceEdge.e2) / 2;
	Vector3d d = (m_sourceEdge.e1 + m_sourceEdge.e2 + m_destEdge.e1 + m_destEdge.e2) / 4;
	glBegin(GL_LINES);
		glColor3d(1,1,0);
		glVertex3d(s.x, s.y, s.z);
		glVertex3d(d.x, d.y, d.z);
	glEnd();
}

}
