/***
 * hesperus: GeomUtil.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "GeomUtil.h"

#include <set>

#include "Sphere.h"
#include "UniquePlanePred.h"

namespace hesp {

//#################### GLOBAL FUNCTIONS ####################
/**
Classifies the line segment p1-p2 against the plane.

@param p1		One endpoint of the line segment
@param p2		The other endpoint of the line segment
@param plane	The plane against which to classify the line segment
@param cp1		Used to return the classification of p1 against the plane to the caller
@param cp2		Used to return the classification of p2 against the plane to the caller
@return			CP_BACK, if the line segment is behind the plane; CP_COPLANAR, if it is on the plane; CP_FRONT, if it is in front of the plane;
				CP_STRADDLE, if it straddles the plane
*/
PlaneClassifier classify_linesegment_against_plane(const Vector3d& p1, const Vector3d& p2, const Plane& plane, PlaneClassifier& cp1, PlaneClassifier& cp2)
{
	cp1 = classify_point_against_plane(p1, plane);
	cp2 = classify_point_against_plane(p2, plane);

	bool backFlag = false, frontFlag = false;
	if(cp1 == CP_BACK || cp2 == CP_BACK) backFlag = true;
	if(cp1 == CP_FRONT || cp2 == CP_FRONT) frontFlag = true;

	if(backFlag && frontFlag) return CP_STRADDLE;
	else if(backFlag) return CP_BACK;
	else if(frontFlag) return CP_FRONT;
	else return CP_COPLANAR;
}

/**
Classifies the point p against the plane.

@param p		The point to be classified
@param plane	The plane against which to classify it
@return			CP_BACK, if the point is behind the plane; CP_COPLANAR, if it is on the plane; CP_FRONT, if it is in front of the plane
*/
PlaneClassifier classify_point_against_plane(const Vector3d& p, const Plane& plane)
{
	const Vector3d& n = plane.normal();
	double d = plane.distance_value();

	double value = n.dot(p) - d;

	if(fabs(value) < EPSILON) return CP_COPLANAR;
	else if(value > 0) return CP_FRONT;
	else return CP_BACK;
}

/**
Determines the point of intersection of a line segment with another (non-vertical) line segment, if any.

@param segment		The first line segment (can be arbitrary)
@param nvSegment	The second line segment (must be non-vertical)
@return				The point of intersection, if any, or boost::none otherwise
*/
boost::optional<Vector3d> determine_linesegment_intersection_with_nonvertical_linesegment(const LineSegment3d& segment, const LineSegment3d& nvSegment)
{
	Plane plane = *make_axial_plane(nvSegment.e1, nvSegment.e2, Vector3d(0,0,1));

	// Make sure we don't try and determine the intersection if the segment's parallel to the plane.
	Vector3d v = segment.e2 - segment.e1;
	if(fabs(v.dot(plane.normal())) < EPSILON) return boost::none;

	std::pair<Vector3d,bool> hit = determine_linesegment_intersection_with_plane(segment.e1, segment.e2, plane, false);
	if(!hit.second) return boost::none;

	// We need to check that the point's within the non-vertical edge.
	const Vector3d& p = hit.first;
	Vector3d d = p - nvSegment.e1;
	Vector3d w = nvSegment.e2 - nvSegment.e1;
	double dotProd = d.dot(w);	// == |d||w| cos angle
	double dLength = d.length(), wLength = w.length();
	double cosAngle = dotProd / (dLength * wLength);
	if(fabs(cosAngle - 1) > SMALL_EPSILON) return boost::none;	// d and w aren't pointing in the same direction
	if(dLength > wLength) return boost::none;					// the intersection point's beyond nvSegment.e2

	return p;
}

/**
Given a ray r(t) = s + tv and an axis-aligned bounding box aabb, finds the non-empty interval [t_e,t_l]
for which the ray is inside aabb (if any).

@param s		A point on the ray
@param v		The direction vector of the ray
@param aabb		The axis-aligned bounding box
@return			The non-empty interval for which the ray is inside aabb, if any, or boost::none otherwise
*/
boost::optional<std::pair<double,double> > determine_ray_intersection_parameters(const Vector3d& s, const Vector3d& v, const AABB3d& aabb)
{
	/*
	An AABB is the intersection of three slabs (a slab is the infinite volume between two parallel planes).
	Ignoring special cases, we calculate the entry and leaving t values for each slab, then pick the largest
	entry t value and smallest exit value to define the t interval during which the ray is within the slab.
	*/

	const Vector3d& mins = aabb.minimum();
	const Vector3d& maxs = aabb.maximum();

	double t_E = INT_MIN, t_L = INT_MAX;

	if(fabs(v.x) > EPSILON)
	{
		double t1 = (mins.x - s.x) / v.x;	// t when ray crosses the xmin plane
		double t2 = (maxs.x - s.x) / v.x;	// t when ray crosses the xmax plane
		if(t1 < t2)	{ t_E = t1; t_L = t2; }
		else		{ t_E = t2; t_L = t1; }
	}
	else
	{
		// The ray is parallel to the x slab (i.e. it lies in the y-z plane). If the ray isn't strictly inside the slab, exit early.
		if(s.x <= mins.x || s.x >= maxs.x) return boost::none;
	}

	if(fabs(v.y) > EPSILON)
	{
		double t1 = (mins.y - s.y) / v.y;	// t when ray crosses the ymin plane
		double t2 = (maxs.y - s.y) / v.y;	// t when ray crosses the ymax plane
		if(t1 < t2)	{ if(t1 > t_E) t_E = t1; if(t2 < t_L) t_L = t2; }
		else		{ if(t2 > t_E) t_E = t2; if(t1 < t_L) t_L = t1; }
	}
	else
	{
		// The ray is parallel to the y slab. If the ray isn't strictly inside the slab, exit early.
		if(s.y <= mins.y || s.y >= maxs.y) return boost::none;
	}

	if(fabs(v.z) > EPSILON)
	{
		double t1 = (mins.z - s.z) / v.z;	// t when ray crosses the zmin plane
		double t2 = (maxs.z - s.z) / v.z;	// t when ray crosses the zmax plane
		if(t1 < t2)	{ if(t1 > t_E) t_E = t1; if(t2 < t_L) t_L = t2; }
		else		{ if(t2 > t_E) t_E = t2; if(t1 < t_L) t_L = t1; }
	}
	else
	{
		// The ray is parallel to the z slab. If the ray isn't strictly inside the slab, exit early.
		if(s.z <= mins.z || s.z >= maxs.z) return boost::none;		
	}

	// If the t interval during which the ray is within the AABB is non-empty, return it. Otherwise, return boost::none.
	if(t_E < t_L) return std::make_pair(t_E, t_L);
	else return boost::none;
}

/**
Given a ray r(t) = s + tv and a sphere, finds the non-empty interval [t_e,t_l]
for which the ray is inside the sphere (if any).

@param s		A point on the ray
@param v		The direction vector of the ray
@param sphere	The sphere
@return			The non-empty interval for which the ray is inside the sphere, if any, or boost::none otherwise
*/
boost::optional<std::pair<double,double> > determine_ray_intersection_parameters(const Vector3d& s, const Vector3d& v, const Sphere& sphere)
{
	/*
	Letting C be the centre of the sphere, and R be its radius, we need to solve (for t) the quadratic equation:

	|r(t) - C|^2 - R^2 = 0

	Letting k = s - C, and working through the maths:

	|s + tv - C|^2 - R^2 = 0
	|k + tv|^2 - R^2 = 0
	(kx + t.vx)^2 + (ky + t.vy)^2 + (kz + t.vz)^2 - R^2 = 0
	t^2(vx^2 + vy^2 + vz^2) + t(2kx.vx + 2ky.vy + 2kz.vz) + (kx^2 + ky^2 + kz^2 - R^2) = 0
	t^2(v.v) + t(2k.v) + (k.k - R^2) = 0
	*/

	Vector3d k = s - sphere.centre();
	double R = sphere.radius();

	double a = v.dot(v);
	double b = 2*k.dot(v);
	double c = k.dot(k) - R*R;

	double discrim = b*b - 4*a*c;

	// If the ray doesn't strictly intersect the sphere, exit early.
	if(discrim <= 0) return boost::none;

	double denom = 2*a;
	double sqrtDiscrim = sqrt(discrim);
	double tE = (-b - sqrtDiscrim) / denom, tL = (-b + sqrtDiscrim) / denom;

	assert(tE < tL);

	return std::make_pair(tE, tL);
}

/**
Calculates the perpendicular displacement of the point p from the plane.

@param p		The point whose perpendicular displacement from the plane we wish to determine
@param plane	The plane
@return			The perpendicular displacement of p from the plane
*/
double displacement_from_plane(const Vector3d& p, const Plane& plane)
{
	const Vector3d& n = plane.normal();
	double d = plane.distance_value();

	// Note that this equation is valid precisely because the plane normal is guaranteed to be unit length.
	return n.dot(p) - d;
}

/**
Calculates the perpendicular distance between the point p and the plane.

@param p		The point whose perpendicular distance from the plane we wish to determine
@param plane	The plane
@return			The perpendicular distance between p and the plane
*/
double distance_to_plane(const Vector3d& p, const Plane& plane)
{
	return fabs(displacement_from_plane(p, plane));
}

/**
Finds the unique set of planes from an initial list which may contain duplicates.
Note that planes which differ only in the orientation of their normal are treated
as equivalent here.

@param planes	The initial list of planes
@return			As stated
*/
std::list<Plane_CPtr> find_unique_planes(const std::list<Plane_CPtr>& planes)
{
	typedef std::set<Plane, UniquePlanePred> UniquePlaneSet;

	const double angleTolerance = 0.5 * PI / 180;	// convert 0.5 degrees to radians
	const double distTolerance = 0.001;
	UniquePlaneSet uniquePlanes(UniquePlanePred(angleTolerance, distTolerance));

	for(std::list<Plane_CPtr>::const_iterator it=planes.begin(), iend=planes.end(); it!=iend; ++it)
	{
		uniquePlanes.insert((*it)->to_undirected_form());
	}

	std::list<Plane_CPtr> ret;
	for(UniquePlaneSet::const_iterator it=uniquePlanes.begin(), iend=uniquePlanes.end(); it!=iend; ++it)
	{
		ret.push_back(Plane_CPtr(new Plane(*it)));
	}
	return ret;
}

/**
Returns an arbitrary unit vector perpendicular to the specified plane's normal.

@param plane	The plane to which our arbitrary unit vector is to be coplanar
@return			An arbitrary unit vector v such that v.dot(plane.normal()) == 0
*/
Vector3d generate_arbitrary_coplanar_unit_vector(const Plane& plane)
{
	return generate_specific_coplanar_unit_vector(plane);
}

/**
Returns the normalized cross product of the specified plane's normal and
another vector which is non-parallel to the normal. This vector will be
the up vector (0,0,1), unless that is parallel to the normal, in which
case (1,0,0) will be used instead. The resulting vector lies in the plane.

@param plane	The plane in which the result vector is to lie
@return			The unit coplanar vector v as specified, satisfying v.dot(plane.normal()) == 0
*/
Vector3d generate_specific_coplanar_unit_vector(const Plane& plane)
{
	const Vector3d& n = plane.normal();
	Vector3d up(0,0,1);
	if(fabs(n.x) < EPSILON && fabs(n.y) < EPSILON)
	{
		// Special Case: n is too close to the vertical and hence n x up is roughly equal to (0,0,0)

		// Use (1,0,0) instead of up and apply the same method as in the else clause.
		return n.cross(Vector3d(1,0,0)).normalize();
	}
	else
	{
		// The normalized cross product of n and up satisfies the requirements of being
		// unit length and perpendicular to n (since we dealt with the special case where
		// n x up is zero, in all other cases it must be non-zero and we can normalize it
		// to give us a unit vector)
		return n.cross(up).normalize();
	}
}

/**
Attempts to construct the plane in which v1, v2 and v1+axis all lie.

@param p1		A point
@param p2		Another point
@param axis		A vector (for our purposes, this will always be an axis vector like (0,0,1))
@return			The constructed plane, if p2-p1 and axis aren't parallel vectors, or NULL otherwise
*/
Plane_Ptr make_axial_plane(const Vector3d& p1, const Vector3d& p2, const Vector3d& axis)
{
	Vector3d v = p2 - p1;
	Vector3d n = v.cross(axis);

	if(n.length_squared() < EPSILON*EPSILON) return Plane_Ptr();
	else return Plane_Ptr(new Plane(n, p1));
}

/**
Returns the point in the specified plane which is nearest to the specified point.

@param p		The point whose nearest point in the plane we want to find
@param plane	The plane
@return			A point r s.t. classify_point_against_plane(r, plane) == CP_COPLANAR and there
				is no point r' s.t. classify_point_against_plane(r', plane) == CP_COPLANAR &&
				p.distance(r') < p.distance(r)
*/
Vector3d nearest_point_in_plane(const Vector3d& p, const Plane& plane)
{
	/*
	Derivation of the algorithm:

	The nearest point in the plane is the point we get if we head from p towards the plane
	along the normal.
	*/

	double displacement = displacement_from_plane(p, plane);
	return p - displacement * plane.normal();
}

/**
Projects a vector onto a plane.

@param v		The vector to be projected
@param plane	The plane onto which to project it
@return			A planar vector representing the result of the projection
*/
Vector3d project_vector_onto_plane(const Vector3d& v, const Plane& plane)
{
	Vector3d normalComponent = v.project_onto(plane.normal());
	return v - normalComponent;
}

//################## HELPER METHODS FOR THE split_polygon FUNCTION ##################
/**
Returns the index of the next vertex (in winding order) after the one specified.

@param index		Some vertex index
@param vertCount	The number of vertices in the polygon
@return				The index of the next vertex after the one specified
*/
int next_vert(int index, int vertCount)
{
	return (index+1)%vertCount;
}

/**
Returns the index of the previous vertex (in winding order) before the one specified.

@param index		Some vertex index
@param vertCount	The number of vertices in the polygon
@return				The index of the previous vertex before the one specified
*/
int prev_vert(int index, int vertCount)
{
	return (index+vertCount-1)%vertCount;
}

}
