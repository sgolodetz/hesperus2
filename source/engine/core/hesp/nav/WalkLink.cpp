/***
 * hesperus: WalkLink.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "WalkLink.h"

#include <sstream>

#include <hesp/ogl/WrappedGL.h>

#include <hesp/math/geom/GeomUtil.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
WalkLink::WalkLink(int sourcePoly, int destPoly, const Vector3d& p1, const Vector3d& p2)
:	NavLink(sourcePoly, destPoly), m_edge(p1, p2)
{}

WalkLink::WalkLink(int sourcePoly, int destPoly, const LineSegment3d& edge)
:	NavLink(sourcePoly, destPoly), m_edge(edge)
{}

//#################### PUBLIC METHODS ####################
Vector3d WalkLink::dest_position() const
{
	return (m_edge.e1 + m_edge.e2) / 2;
}

boost::optional<Vector3d> WalkLink::hit_test(const Vector3d& s, const Vector3d& d) const
{
	return determine_linesegment_intersection_with_nonvertical_linesegment(LineSegment3d(s,d), m_edge);
}

NavLink_Ptr WalkLink::load(const std::string& data)
{
	std::stringstream ss;
	ss << data;

	std::ios_base::fmtflags oldFlags = ss.flags();
	ss.setf(std::ios_base::skipws);

	int sourcePoly, destPoly;
	LineSegment3d edge;

	ss >> sourcePoly >> destPoly >> edge;

	ss.flags(oldFlags);

	return NavLink_Ptr(new WalkLink(sourcePoly, destPoly, edge));
}

void WalkLink::output(std::ostream& os) const
{
	os << "Walk " << m_sourcePoly << ' ' << m_destPoly << ' ' << m_edge;
}

void WalkLink::render() const
{
	Vector3d mid = (m_edge.e1 + m_edge.e2) / 2;
	Vector3d alongEdge = (m_edge.e2 - m_edge.e1);
	Vector3d acrossEdge = alongEdge.cross(Vector3d(0,0,1)).normalize();
	acrossEdge *= 0.2;
	alongEdge *= 0.1;
	Vector3d s1 = mid + acrossEdge - alongEdge, s2 = mid + acrossEdge + alongEdge;
	Vector3d d1 = mid - acrossEdge - alongEdge, d2 = mid - acrossEdge + alongEdge;
	glDisable(GL_CULL_FACE);
	glBegin(GL_POLYGON);
		glColor3d(0,1,1);
		glVertex3d(s1.x, s1.y, s1.z);
		glVertex3d(s2.x, s2.y, s2.z);
		glVertex3d(d2.x, d2.y, d2.z);
		glVertex3d(d1.x, d1.y, d1.z);
	glEnd();
	glEnable(GL_CULL_FACE);
}

Vector3d WalkLink::source_position() const
{
	return (m_edge.e1 + m_edge.e2) / 2;
}

double WalkLink::traversal_time(double traversalSpeed) const
{
	return 0;
}

Vector3d WalkLink::traverse(const Vector3d& source, double t) const
{
	return source;
}

}
