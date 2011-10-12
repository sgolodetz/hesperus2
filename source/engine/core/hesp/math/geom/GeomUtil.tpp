/***
 * hesperus: GeomUtil.tpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include <cmath>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>

#include <hesp/exceptions/InvalidParameterException.h>
#include <hesp/math/Constants.h>

namespace hesp {

//#################### GLOBAL FUNCTIONS ####################
/**
Classifies the polygon poly against the plane.

@param poly		The polygon to be classified
@param plane	The plane against which to classify it
@return			CP_BACK, if the polygon is entirely behind the plane; CP_COPLANAR, if it lies in the plane;
				CP_FRONT, if it is entirely in front of the plane; CP_STRADDLE, if it straddles the plane
*/
template <typename Vert, typename AuxData>
PlaneClassifier classify_polygon_against_plane(const Polygon<Vert,AuxData>& poly, const Plane& plane)
{
	bool backFlag = false, frontFlag = false;

	// Checking the undirected plane of the polygon against the undirected form of the plane
	// first makes the test more robust in the case of very nearly coplanar polygons. This
	// can really matter in cases where the classification of the polygon as coplanar is
	// crucial, e.g. during portal generation.
	Plane polyPlane = make_plane(poly).to_undirected_form();
	Plane undirectedPlane = plane.to_undirected_form();
	double dotProd = polyPlane.normal().dot(undirectedPlane.normal());
	double distDelta = polyPlane.distance_value() - undirectedPlane.distance_value();
	if(fabs(dotProd-1) < EPSILON && fabs(distDelta) < EPSILON) return CP_COPLANAR;

	int vertCount = poly.vertex_count();
	for(int i=0; i<vertCount; ++i)
	{
		PlaneClassifier cp = classify_point_against_plane(poly.vertex(i), plane);
		switch(cp)
		{
		case CP_BACK:
			backFlag = true;
			break;
		case CP_FRONT:
			frontFlag = true;
			break;
		default:
			break;
		}
		if(backFlag && frontFlag) return CP_STRADDLE;
	}

	// If we get here, the polygon doesn't straddle the plane, so at most one of CP_BACK and CP_FRONT is true.
	if(backFlag) return CP_BACK;
	else if(frontFlag) return CP_FRONT;
	else return CP_COPLANAR;
}

/**
Constructs a bounding box around an array of polygons.

@param polys	The polygons
@return			The bounding box
*/
template <typename Vert, typename AuxData>
AABB3d construct_bounding_box(const std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polys)
{
	Vector3d minimum(INT_MAX, INT_MAX, INT_MAX), maximum(INT_MIN, INT_MIN, INT_MIN);

	int polyCount = static_cast<int>(polys.size());
	for(int i=0; i<polyCount; ++i)
	{
		int vertCount = polys[i]->vertex_count();
		for(int j=0; j<vertCount; ++j)
		{
			const Vector3d& v = polys[i]->vertex(j);
			if(v.x < minimum.x) minimum.x = v.x;	if(v.x > maximum.x) maximum.x = v.x;
			if(v.y < minimum.y) minimum.y = v.y;	if(v.y > maximum.y) maximum.y = v.y;
			if(v.z < minimum.z) minimum.z = v.z;	if(v.z > maximum.z) maximum.z = v.z;
		}
	}

	return AABB3d(minimum, maximum);
}

/**
Given the half-ray r(t) = s + tv [t >= 0] and a shape, finds the points at which
the half-ray enters and leaves the shape (if any).

@param s		The start point of the half-ray
@param v		The direction vector of the half-ray
@param shape	The shape
@return			The pair of (enter,leave) points, if they exist, or boost::none otherwise
*/
template <typename T>
boost::optional<std::pair<Vector3d,Vector3d> > determine_halfray_intersection_with_shape(const Vector3d& s, const Vector3d& v, const T& shape)
{
	boost::optional<std::pair<double,double> > params = determine_ray_intersection_parameters(s, v, shape);

	if(params)
	{
		double t_E = std::max(params->first, 0.0);	// clamp the entry t value (t_E) to 0
		double t_L = params->second;

		// If there's still a bit of ray which intersects the shape (even after clamping to the forward part of it),
		// then calculate the intersection points and return them.
		if(t_E < t_L)
		{
			return std::make_pair(s + t_E * v, s + t_L * v);
		}
	}

	return boost::none;
}

/**
Determines the point at which the line r = s + tv intersects the specified plane.

<p><b>Preconditions:</b>
<dl>
<dd>The line must not be parallel to the plane
<dd>|v| > 0
</dl>

@param s			An arbitrary point on the line
@param v			The direction vector of the line
@param plane		The plane with which to intersect
@return				A pair containing the intersection point of the line with the plane and the
					value of the parameter t at that intersection point
@throws Exception	If the line is parallel to the plane or |v| = 0
*/
template <typename Vec>
std::pair<Vec,double> determine_line_intersection_with_plane(const Vec& s, const Vec& v, const Plane& plane)
{
	/*
	Derivation of the algorithm:

	The plane's equation is n . x - d = 0
	The line's equation is r = s + tv

	We want to find where they intersect, i.e. we want to find t (and hence r) s.t.

	n . r - d = 0

	Well:

	n . (s + tv) - d = 0
	n . s + t(n . v) - d = 0
	t = (d - n . s) / n . v
	r = s + ((d - n . s) / n . v).v

	Of course, there's only a solution if n . v != 0, i.e. if the line isn't parallel to the plane and |v| > 0
	*/

	const Vector3d& n = plane.normal();
	double d = plane.distance_value();

	double nDOTv = n.dot(v);

	// If the line is parallel to the plane or |v| = 0, we've violated our precondition.
	if(nDOTv == 0) throw Exception("n . v == 0");

	double t = (d - n.dot(s)) / nDOTv;
	Vec tv = t*v;
	Vec r = s + tv;
	return std::make_pair(r,t);
}

/**
Determines the point at which the line segment p1-p2 intersects the specified plane.

<p><b>Preconditions:</b>
<dl>
<dd>The line segment must not be parallel to the plane
<dd>The two points must not be identical
</dl>

@param p1			One endpoint of the line segment
@param p2			The other endpoint of the line segment
@param plane		The plane with which to intersect
@param strict		A boolean specifying whether the intersection point must be *strictly* between the endpoints or not
@return				A pair containing the intersection point of the line in which the line segment lies with
					the plane and a boolean indicating whether or not it was between the endpoints
@throws Exception	If the line segment is parallel to the plane or the points are identical
*/
template <typename Vec>
std::pair<Vec,bool> determine_linesegment_intersection_with_plane(const Vec& p1, const Vec& p2, const Plane& plane, bool strict)
{
	/*
	Derivation of the algorithm:

	The intersection of a line segment with a plane can be calculated by intersecting the line in which it
	lies with the plane and checking whether the intersection point is between the specified endpoints of
	the segment. The intersection point returned is (strictly) between the endpoints iff the value of the
	parameter t returned by determine_line_intersection_with_plane satisfies 0 < t < 1. The corresponding
	non-strict test is 0 <= t <= 1.
	*/

	std::pair<Vec,double> intersection = determine_line_intersection_with_plane(p1, p2 - p1, plane);
	return std::make_pair(intersection.first, strict ? 0 < intersection.second && intersection.second < 1 : 0 <= intersection.second && intersection.second <= 1);
}

/**
Makes a Plane object to represent the plane in which the specified polygon lies.

@poly	The polygon whose plane we wish to make
@return	The plane
*/
template <typename Vert, typename AuxData>
Plane make_plane(const Polygon<Vert,AuxData>& poly)
{
	return Plane(poly.normal(), poly.vertex(0));
}

/**
Returns a polygon on the specified plane which is large enough to
span the entire level space.

@param plane	The plane for which we want to make a universe polygon
@return			A polygon poly s.t. classify_polygon_against_plane(poly, plane) == CP_COPLANAR
				and the angle between the polygon and plane normals is less than EPSILON and
				poly is large enough to span the entire level space
*/
template <typename AuxData>
shared_ptr<Polygon<Vector3d,AuxData> > make_universe_polygon(const Plane& plane, const AuxData& auxData)
{
	typedef Polygon<Vector3d,AuxData> Poly;
	typedef shared_ptr<Poly> Poly_Ptr;

	Vector3d origin(0,0,0);
	Vector3d centre = nearest_point_in_plane(origin, plane);

	Vector3d planarVecs[2];
	planarVecs[0] = generate_arbitrary_coplanar_unit_vector(plane);
	planarVecs[1] = planarVecs[0].cross(plane.normal()).normalize();

	const double HALFSIDELENGTH = 1000000;	// something arbitrarily huge (but not too big, to avoid floating-point issues)
	for(int i=0; i<2; ++i) planarVecs[i] *= HALFSIDELENGTH;

	std::vector<Vector3d> vertices;
	for(int i=0; i<4; ++i) vertices.push_back(centre);
	vertices[0] -= planarVecs[0];	vertices[0] -= planarVecs[1];
	vertices[1] -= planarVecs[0];	vertices[1] += planarVecs[1];
	vertices[2] += planarVecs[0];	vertices[2] += planarVecs[1];
	vertices[3] += planarVecs[0];	vertices[3] -= planarVecs[1];

	return Poly_Ptr(new Poly(vertices, auxData));
}

/**
Determines whether or not p is inside poly (including its edges).

@param p		A point
@param poly		A polygon
@return			true, if p is inside poly, or false otherwise
*/
template <typename Vert, typename AuxData>
bool point_in_polygon(const Vector3d& p, const Polygon<Vert,AuxData>& poly)
{
	if(classify_point_against_plane(p, make_plane(poly)) != CP_COPLANAR) return false;

	const Vector3d zero(0,0,0);

	std::vector<Vector3d> relVecs;	// position of vertices relative to p
	int vertCount = poly.vertex_count();
	relVecs.reserve(vertCount);
	for(int i=0; i<vertCount; ++i)
	{
		relVecs.push_back(Vector3d(poly.vertex(i)) - p);
		if(relVecs[i].length_squared() < EPSILON*EPSILON)
		{
			// p is one of the vertices of poly
			return true;
		}
		else relVecs[i].normalize();
	}

	double angleSum = 0;
	for(int i=0; i<vertCount; ++i)
	{
		int j = (i+1)%vertCount;
		double dotProd = relVecs[i].dot(relVecs[j]);

		// Implicitly clamp the result of the dot product to the range [-1,1] to avoid problems with rounding errors when we call acos.
		// Note that if dotProd = 1, acos(1) = 0, which is why this works.
		if(dotProd <= -1) angleSum += PI;
		else if(dotProd < 1) angleSum += acos(relVecs[i].dot(relVecs[j]));
	}

	return fabs(angleSum - 2*PI) < EPSILON;
}

/**
Splits the polygon across the specified plane.

@param poly							The polygon to split across the plane
@param plane						The plane across which to split the polygon
@return								A SplitResults object containing the results of the split
@throws InvalidParameterException	If the polygon doesn't straddle the plane
*/
template <typename Vert, typename AuxData>
SplitResults<Vert,AuxData> split_polygon(const Polygon<Vert,AuxData>& poly, const Plane& plane)
{
	if(classify_polygon_against_plane(poly, plane) != CP_STRADDLE) throw InvalidParameterException("Polygon doesn't straddle plane");

	std::vector<Vert> backHalf, frontHalf;

	// Find a start vertex which isn't on the plane. If there isn't one, the polygon doesn't
	// straddle the plane, so we've violated a precondition (but we just checked this above,
	// so it shouldn't happen).
	int startVert = -1;
	PlaneClassifier cp;
	int vertCount = poly.vertex_count();
	for(int i=0; i<vertCount; ++i)
	{
		cp = classify_point_against_plane(poly.vertex(i), plane);
		if(cp != CP_COPLANAR)
		{
			startVert = i;
			break;
		}
	}
	if(startVert == -1) throw InvalidParameterException("Polygon lies in plane: unable to find a non-coplanar start vertex for polygon splitting");

	// At this point, cp contains the classification of poly.vertex(startVert) against the plane
	// Note that cp != CP_COPLANAR

	switch(cp)
	{
		case CP_BACK:
		{
			std::pair<int,PlaneClassifier> p = construct_half_polygon(backHalf, frontHalf, poly, plane, startVert, cp);
			p = construct_half_polygon(frontHalf, backHalf, poly, plane, p.first, p.second);
			complete_original_half(backHalf, poly, p.first, startVert);
			break;
		}
		case CP_FRONT:
		{
			std::pair<int,PlaneClassifier> p = construct_half_polygon(frontHalf, backHalf, poly, plane, startVert, cp);
			p = construct_half_polygon(backHalf, frontHalf, poly, plane, p.first, p.second);
			complete_original_half(frontHalf, poly, p.first, startVert);
			break;
		}
	}

	// Construct the polygons from the lists of vertices and the auxiliary data.
	// Note that the auxiliary data is simply inherited from the parent polygon:
	// it may or may not be necessary to allow this behaviour to be customised
	// in the future.
	typedef Polygon<Vert,AuxData> Poly;
	typedef shared_ptr<Poly> Poly_Ptr;
	Poly_Ptr backPoly(new Poly(backHalf, poly.auxiliary_data()));
	Poly_Ptr frontPoly(new Poly(frontHalf, poly.auxiliary_data()));

	return SplitResults<Vert,AuxData>(backPoly, frontPoly);
}

//################## HELPER METHODS FOR THE split_polygon FUNCTION ##################
/**
Completes the original half of the split polygon (the side we did first). (It's used, obviously, in the
split_polygon function.)

@param originalHalf	The vertex list for the side of the split polygon we constructed first
@param poly			The polygon we're splitting
@param i			The index of the vertex we're currently processing
@param startVert	The first vertex we processed
*/
template <typename Vert, typename AuxData>
void complete_original_half(std::vector<Vert>& originalHalf, const Polygon<Vert,AuxData>& poly, int i, int startVert)
{
	const int vertCount = poly.vertex_count();

	while(i != startVert)
	{
		originalHalf.push_back(poly.vertex(i));
		i = next_vert(i, vertCount);
	}
}

/**
Constructs one of the two halves of the split polygon. (It's used, obviously, in the split_polygon function.)

<p><b>Preconditions:</b>
<dl>
<dd>cp == CP_BACK or CP_FRONT (initially, at any rate - it does get modified)
<dd>currentHalf != otherHalf
<dd>0 <= i < poly.vertex_count()
</dl>

@param currentHalf	The vertices for the half of the polygon we're currently constructing
@param otherHalf	The vertices for the other half of the polygon
@param poly			The polygon we're splitting
@param plane		The splitting plane
@param i			The index of the vertex we're currently processing
@param cp			The classification of that vertex against the splitting plane
@return				A pair, the first component of which is the new value of i and the second of which is the new value of cp
*/
template <typename Vert, typename AuxData>
std::pair<int,PlaneClassifier> construct_half_polygon(std::vector<Vert>& currentHalf, std::vector<Vert>& otherHalf, const Polygon<Vert,AuxData>& poly,
													  const Plane& plane, int i, PlaneClassifier cp)
{
	const int vertCount = poly.vertex_count();

	PlaneClassifier oldcp;	// stores the classification of the previous vertex against the plane (initially indeterminate)
	do
	{
		currentHalf.push_back(poly.vertex(i));
		i = next_vert(i, vertCount);
		oldcp = cp;
		cp = classify_point_against_plane(poly.vertex(i), plane);
	} while(cp == oldcp);

	// If two consecutive points aren't on the same side of the plane and the first one
	// isn't coplanar, either the second one's coplanar or it's on the opposite side of
	// the plane from the first.
	if(cp == CP_COPLANAR)
	{
		currentHalf.push_back(poly.vertex(i));
		otherHalf.push_back(poly.vertex(i));
		i = next_vert(i, vertCount);
		cp = classify_point_against_plane(poly.vertex(i), plane);

		// Having two or more coplanar points in sequence would imply that the polygon
		// doesn't straddle the plane, which violates our precondition. However, in
		// practice, this can occur if we have two or more points that are very close
		// together and almost on the plane: due to the reasonably large tolerance value,
		// they all get treated as being actually on the plane. We therefore choose to
		// handle this situation by simply skipping additional coplanar vertices in this case.
		while(cp == CP_COPLANAR)
		{
			i = next_vert(i, vertCount);
			cp = classify_point_against_plane(poly.vertex(i), plane);
		}
	}
	else
	{
		Vert intersectionPoint = determine_linesegment_intersection_with_plane(poly.vertex(prev_vert(i, vertCount)), poly.vertex(i), plane).first;
		currentHalf.push_back(intersectionPoint);
		otherHalf.push_back(intersectionPoint);
	}

	// Note: At this point, the vertex with index i is in the other half of the polygon
	// (i.e. non-coplanar). Its classification against the plane is cp.
	return std::make_pair(i, cp);
}

}
