/***
 * hesperus: Antipenumbra.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "Antipenumbra.h"

#include <hesp/math/geom/GeomUtil.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
/**
Constructs an antipenumbra from a source portal to a target portal.
For each antipenumbral plane p (except the source plane), it is
guaranteed to be the case that:

- classify_polygon_against_plane(*source, p) == CP_BACK
- classify_polygon_against_plane(*target, p) == CP_FRONT

@param source	The source portal
@param target	The target portal
*/
Antipenumbra::Antipenumbra(const Portal_Ptr& source, const Portal_Ptr& target)
{
	m_planes.push_back(make_plane(*source));

	// Note: In both cases here, source lies behind the generated planes and target lies in front of them.
	add_clip_planes(source, target, CP_BACK);	// add planes from source to target, with source behind them
	add_clip_planes(target, source, CP_FRONT);	// add planes from target to source, with target in front of them
}

//#################### PRIVATE METHODS ####################
/**
Adds clip planes which separate portal 'from' and portal 'to'. The classifier
specifies on which side of the generated planes portal 'from' should lie.

@param from						The from portal
@param to						The to portal
@param desiredFromClassifier	The side of the planes on which portal from should lie
*/
void Antipenumbra::add_clip_planes(const Portal_CPtr& from, const Portal_CPtr& to, PlaneClassifier desiredFromClassifier)
{
	int fromCount = from->vertex_count();
	int toCount = to->vertex_count();

	for(int i=0; i<fromCount; ++i)
	{
		const Vector3d& a = from->vertex(i);
		const Vector3d& b = from->vertex((i+1)%fromCount);
		for(int j=0; j<toCount; ++j)
		{
			const Vector3d& c = to->vertex(j);
			Plane_Ptr plane = construct_clip_plane(a, b, c);
			if(!plane) continue;

			PlaneClassifier cpFrom = classify_polygon_against_plane(*from, *plane);
			if(cpFrom == CP_BACK || cpFrom == CP_FRONT)
			{
				PlaneClassifier cpTo = classify_polygon_against_plane(*to, *plane);
				if((cpTo == CP_BACK || cpTo == CP_FRONT) && cpTo != cpFrom)
				{
					// If we get here, either cpFrom == CP_BACK && cpTo == CP_FRONT, or vice-versa.
					if(cpFrom != desiredFromClassifier) m_planes.push_back(plane->flip());
					else m_planes.push_back(*plane);
					break;
				}
			}
		}
	}
}

/**
Returns the plane through a, b and c.

@param a	The first vector in the plane
@param b	The second vector in the plane
@param c	The third vector in the plane
@return		As stated
*/
Plane_Ptr Antipenumbra::construct_clip_plane(const Vector3d& a, const Vector3d& b, const Vector3d& c)
{
	Vector3d v1 = b - a;
	Vector3d v2 = c - a;

	Vector3d n = v1.cross(v2);
	if(n.length_squared() < EPSILON) return Plane_Ptr();

	return Plane_Ptr(new Plane(n,a));
}

}
