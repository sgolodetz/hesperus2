/***
 * hesperus: StepLink.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "StepLink.h"

#include <hesp/math/geom/GeomUtil.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
StepLink::StepLink(int sourcePoly, int destPoly, const Vector3d& s1, const Vector3d& s2, const Vector3d& d1, const Vector3d& d2)
:	NavLink(sourcePoly, destPoly), m_sourceEdge(s1,s2), m_destEdge(d1,d2)
{}

StepLink::StepLink(int sourcePoly, int destPoly, const LineSegment3d& sourceEdge, const LineSegment3d& destEdge)
:	NavLink(sourcePoly, destPoly), m_sourceEdge(sourceEdge), m_destEdge(destEdge)
{}

//#################### PUBLIC METHODS ####################
Vector3d StepLink::dest_position() const
{
	return (m_destEdge.e1 + m_destEdge.e2) / 2;
}

boost::optional<Vector3d> StepLink::hit_test(const Vector3d& s, const Vector3d& d) const
{
	return determine_linesegment_intersection_with_nonvertical_linesegment(LineSegment3d(s,d), m_sourceEdge);
}

void StepLink::output(std::ostream& os) const
{
	os << link_name() << ' ' << m_sourcePoly << ' ' << m_destPoly << ' ' << m_sourceEdge << ' ' << m_destEdge;
}

Vector3d StepLink::source_position() const
{
	return (m_sourceEdge.e1 + m_sourceEdge.e2) / 2;
}

double StepLink::traversal_time(double traversalSpeed) const
{
	return 0;
}

Vector3d StepLink::traverse(const Vector3d& source, double t) const
{
	double u = (source - m_sourceEdge.e1).length() / (m_sourceEdge.e2 - m_sourceEdge.e1).length();
	return (1-u) * m_destEdge.e1 + u * m_destEdge.e2;
}

}
