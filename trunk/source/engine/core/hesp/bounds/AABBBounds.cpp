/***
 * hesperus: AABBBounds.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "AABBBounds.h"

#include <hesp/ogl/WrappedGL.h>

#include <hesp/math/geom/GeomUtil.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
AABBBounds::AABBBounds(const Vector3d& scale)
:	m_scale(scale)
{}

//#################### PUBLIC METHODS ####################
double AABBBounds::brush_expansion_distance(const Vector3d& n) const
{
	Vector3d v = support_point(-n);

	// v . -n = |v| cos theta (see p.26 of J.M.P. van Waveren's thesis on the Q3 bot for a diagram)
	return v.dot(-n);
}

boost::optional<std::pair<Vector3d,Vector3d> > AABBBounds::determine_halfray_intersection(const Vector3d& s, const Vector3d& v) const
{
	AABB3d aabb(-m_scale, m_scale);
	return determine_halfray_intersection_with_shape(s, v, aabb);
}

Vector3d AABBBounds::half_dimensions() const
{
	return m_scale;
}

double AABBBounds::height() const
{
	return m_scale.z * 2;
}

void AABBBounds::render(const Vector3d& pos) const
{
	Vector3d mins = pos - m_scale, maxs = pos + m_scale;

	glPushAttrib(GL_ENABLE_BIT | GL_POLYGON_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
	glColor3d(1,1,0);

	glBegin(GL_QUADS);
		// Front
		glVertex3d(mins.x, mins.y, mins.z);
		glVertex3d(maxs.x, mins.y, mins.z);
		glVertex3d(maxs.x, mins.y, maxs.z);
		glVertex3d(mins.x, mins.y, maxs.z);

		// Right
		glVertex3d(maxs.x, mins.y, mins.z);
		glVertex3d(maxs.x, maxs.y, mins.z);
		glVertex3d(maxs.x, maxs.y, maxs.z);
		glVertex3d(maxs.x, mins.y, maxs.z);

		// Back
		glVertex3d(maxs.x, maxs.y, mins.z);
		glVertex3d(mins.x, maxs.y, mins.z);
		glVertex3d(mins.x, maxs.y, maxs.z);
		glVertex3d(maxs.x, maxs.y, maxs.z);

		// Left
		glVertex3d(mins.x, maxs.y, mins.z);
		glVertex3d(mins.x, mins.y, mins.z);
		glVertex3d(mins.x, mins.y, maxs.z);
		glVertex3d(mins.x, maxs.y, maxs.z);
	glEnd();

	glPopAttrib();
}

Vector3d AABBBounds::support_point(const Vector3d& n) const
{
	double sx = m_scale.x, sy = m_scale.y, sz = m_scale.z;

	int xCode = n.x > 0 ? 4 : 0;
	int yCode = n.y > 0 ? 2 : 0;
	int zCode = n.z > 0 ? 1 : 0;
	int fullCode = xCode + yCode + zCode;

	Vector3d v;
	switch(fullCode)
	{
		case 0:		v = Vector3d(-sx, -sy, -sz);	break;	// n: x-, y-, z-
		case 1:		v = Vector3d(-sx, -sy, sz);		break;	// n: x-, y-, z+
		case 2:		v = Vector3d(-sx, sy, -sz);		break;	// n: x-, y+, z-
		case 3:		v = Vector3d(-sx, sy, sz);		break;	// n: x-, y+, z+
		case 4:		v = Vector3d(sx, -sy, -sz);		break;	// n: x+, y-, z-
		case 5:		v = Vector3d(sx, -sy, sz);		break;	// n: x+, y-, z+
		case 6:		v = Vector3d(sx, sy, -sz);		break;	// n: x+, y+, z-
		default:	v = Vector3d(sx, sy, sz);		break;	// n: x+, y+, z+ (case 7)
	}
	return v;
}

}
