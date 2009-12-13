package MapEditor.Geom.Planar;

import MapEditor.Math.Vectors.*;
import MapEditor.Misc.*;
import MapEditor.Test.*;
import MapEditor.Textures.TexturePlane;
import java.util.LinkedList;

/**
This class contains all the geometric utility functions.
*/
final public class GeomUtil implements Constants, GeomConstants
{
	/**
	Returns a String corresponding to the specified point membership classification constant.

	@param pmc	A point membership classification constant: one of PMC_INSIDE, PMC_OUTSIDE or PMC_SURFACE
	@return		A corresponding String as specified
	@throws java.lang.Error	If pmc is not one of the specified constants
	*/
	private static String PMC_String(int pmc)
	{
		switch(pmc)
		{
			case PMC_INSIDE:
				return "PMC_INSIDE";
			case PMC_OUTSIDE:
				return "PMC_OUTSIDE";
			case PMC_SURFACE:
				return "PMC_SURFACE";
			default:
				throw new java.lang.Error();
		}
	}

	/**
	Classifies a point relative to a plane.

	@param p		The point to classify
	@param plane	The plane against which to classify it
	@return			An integer (unfortunately) giving the point's classification; this will be
					one of CP_BACK, CP_COPLANAR or CP_FRONT
	*/
	public static int classify_point_against_plane(final Vector3d p, final Plane plane)
	{
		double displacement = plane.displacement_to_point(p);
		if(Math.abs(displacement) < EPSILON) return CP_COPLANAR;
		else if(displacement > 0) return CP_FRONT;
		else return CP_BACK;
	}

	/**
	Classifies a polygon relative to a plane.

	@param poly		The polygon to classify
	@param plane	The plane against which to classify it
	@return			An integer (unfortunately) giving the polygon's classification; this will be
					one of CP_BACK, CP_COPLANAR, CP_FRONT or CP_STRADDLE
	*/
	public static int classify_polygon_against_plane(final Polygon poly, final Plane plane)
	{
		boolean backFlag = false, frontFlag = false;

		for(Vector3d v: poly.get_vertices())
		{
			switch(classify_point_against_plane(v, plane))
			{
				case CP_BACK:
				{
					if(frontFlag) return CP_STRADDLE;
					else backFlag = true;
					break;
				}
				case CP_COPLANAR:
				{
					// No effect.
					break;
				}
				case CP_FRONT:
				{
					if(backFlag) return CP_STRADDLE;
					else frontFlag = true;
					break;
				}
			}
		}

		if(backFlag) return CP_BACK;
		else if(frontFlag) return CP_FRONT;
		else return CP_COPLANAR;
	}

	/**
	Constructs an outward-facing, axis-aligned cuboid that encloses a volume between x = left and x = right, y = front and y = back, and
	z = bottom and z = top.

	<p><b>Preconditions:</b>
	<dl>
	<dd>left < right
	<dd>front < back
	<dd>bottom < top
	</dl>

	@param left		The x coordinate of the left bounding plane (which, of course, is a plane of the form x = k)
	@param right	The x coordinate of the right bounding plane
	@param front	The y coordinate of the front bounding plane
	@param back		The y coordinate of the back bounding plane
	@param bottom	The z coordinate of the bottom bounding plane
	@param top		The z coordinate of the top bounding plane
	@return			The cuboid's polygons as a linked list
	*/
	public static LinkedList<Polygon> construct_cuboid(double left, double right, double front, double back, double bottom, double top)
	{
		LinkedList<Polygon> polys = new LinkedList<Polygon>();
		polys.add(new Polygon(new Vector3d[] {	new Vector3d(left,front,bottom),		new Vector3d(left,front,top),		// left
												new Vector3d(left,back,top),			new Vector3d(left,back,bottom)		}));

		polys.add(new Polygon(new Vector3d[] {	new Vector3d(right,front,bottom),	new Vector3d(right,back,bottom),		// right
												new Vector3d(right,back,top),		new Vector3d(right,front,top)		}));

		polys.add(new Polygon(new Vector3d[] {	new Vector3d(left,front,bottom),		new Vector3d(right,front,bottom),	// front
												new Vector3d(right,front,top),		new Vector3d(left,front,top)			}));

		polys.add(new Polygon(new Vector3d[] {	new Vector3d(right,back,bottom),		new Vector3d(left,back,bottom),		// back
												new Vector3d(left,back,top),			new Vector3d(right,back,top)			}));

		polys.add(new Polygon(new Vector3d[] {	new Vector3d(left,front,top),		new Vector3d(right,front,top),		// top
												new Vector3d(right,back,top),		new Vector3d(left,back,top)			}));

		polys.add(new Polygon(new Vector3d[] {	new Vector3d(left,back,bottom),		new Vector3d(right,back,bottom),		// bottom
												new Vector3d(right,front,bottom),	new Vector3d(left,front,bottom)		}));

		return polys;
	}

	/**
	Determines the point at which the line r = s + tv intersects the specified plane.

	<p><b>Preconditions:</b>
	<dl>
	<dd>The line must not be parallel to the plane
	<dd>|v| > 0
	</dl>

	@param s				An arbitrary point on the line
	@param v				The direction vector of the line
	@param plane			The plane with which to intersect
	@return					A pair containing the intersection point of the line with the plane and the
							value of the parameter t at that intersection point
	@throws java.lang.Error	If the line is parallel to the plane or |v| = 0
	*/
	public static Pair<Vector3d,Double> determine_line_intersection_with_plane(final Vector3d s, final Vector3d v,
																			  final Plane plane)
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

		Vector3d n = plane.get_normal();
		double d = plane.get_distance_value();

		double nDOTv = n.dot(v);

		// If the line is parallel to the plane or |v| = 0, we've violated our precondition.
		if(nDOTv == 0) throw new java.lang.Error();

		double t = (d - n.dot(s)) / nDOTv;
		Vector3d tv = v.clone();
		tv.scale(t);

		Vector3d r = s.clone();
		r.add(tv);
		return Pair.make_pair(r, t);
	}

	/**
	Determines the point at which the line segment p1-p2 intersects the line r = a + tv.

	<p><b>Preconditions:</b>
	<dl>
	<dd>The line segment must not be parallel to the line
	<dd>The two points must not be identical
	</dl>

	@param p1				One endpoint of the line segment
	@param p2				The other endpoint of the line segment
	@param a				An arbitrary point on the line with which we are calculating the intersection
	@param v				A vector along the line with which we are calculating the intersection
	@return					A pair containing the intersection point of the line in which the line segment lies with
							the line r = a + tv and a boolean indicating whether or not it was (strictly) between the
							endpoints
	@throws java.lang.Error	If the line segment is parallel to the other line or the points are identical
	*/
	public static Pair<Vector2d,Boolean> determine_linesegment_intersection_with_line(final Vector2d p1, final Vector2d p2,
																					 final Vector2d a, final Vector2d v)
	{
		/*
		Derivation of the algorithm:

		Let u = p2-p1
		The line in which p1-p2 lies has the equation r = p1 + su (where s is a parameter)
		The other line has the equation r = a + tv

		We want to find where they intersect, i.e. we want to find s or t s.t.

		p1 + su = a + tv

		In particular, we will need to know the actual value of s (we can do without the
		actual value of t), so we'll find that.

		We have two equations in two unknowns (s and t), namely:

		p1.x + s*u.x = a.x + t*v.x
		p1.y + s*u.y = a.y + t*v.y

		So:
		t = (p1.x - a.x + s*u.x)/v.x (if v.x != 0 -> see special case below)
		-> p1.y + s*u.y = a.y + (p1.x - a.x + s*u.x)*v.y/v.x
		-> s(u.y - u.x*v.y/v.x) = a.y - p1.y + (p1.x - a.x)*v.y/v.x
		-> s = (a.y - p1.y + (p1.x - a.x)*v.y/v.x) / (u.y - u.x*v.y/v.x)
		(Note that if u.y - u.x*v.y/v.x = 0, then u.y*v.x = u.x*v.y, i.e. the two lines are parallel or
		 the two endpoints were identical and hence u = 0 -> in either case, throw an error)

		Special Case (v.x = 0):
		p1.x + s*u.x = a.x
		(If u.x = 0 as well, then we have violated our precondition, for either u and v are parallel or u = 0 -> throw an error)
		s = (a.x - p1.x) / u.x (if u.x != 0)

		Iff 0 < s < 1, then the intersection point is (strictly) between the endpoints.

		Example 1:

		p1 = (0,0), p2 = (2,2), a = (1,-1), v = (0,1)
		u = p2-p1 = (2,2)
		v.x = 0, so s = (a.x - p1.x) / u.x = (1 - 0) / 2 = 0.5
		r = p1 + su = (0,0) + 0.5(2,2) = (1,1)

		Example 2 (the same lines, the other way round):

		p1 = (1,0), p2 = (1,2), a = (0,0), v = (1,1)
		u = p2-p1 = (0,2)
		v.x != 0, so s = (a.y - p1.y + (p1.x - a.x)*v.y/v.x) / (u.y - u.x*v.y/v.x)
		-> s = (0 - 0 + (1 - 0)*1/1) / (2 - 0*1/1) = 0.5
		r = p1 + su = (1,0) + 0.5(0,2) = (1,1)
		*/

		Vector2d u = VectorUtil.subtract(p2, p1);

		double s;
		if(v.x == 0)
		{
			if(u.x == 0) throw new java.lang.Error();
			s = (a.x - p1.x) / u.x;
		}
		else
		{
			double denom = u.y - u.x*v.y/v.x;
			if(denom == 0) throw new java.lang.Error();
			s = (a.y - p1.y + (p1.x - a.x)*v.y/v.x) / denom;
		}

		Vector2d r = Vector2d.scale_add(s, u, p1);	// r = p1 + su
		return Pair.make_pair(r, 0 < s && s < 1);
	}

	/**
	Determines the point at which the line segment p1-p2 intersects the specified plane.

	<p><b>Preconditions:</b>
	<dl>
	<dd>The line segment must not be parallel to the plane
	<dd>The two points must not be identical
	</dl>

	@param p1				One endpoint of the line segment
	@param p2				The other endpoint of the line segment
	@param plane			The plane with which to intersect
	@return					A pair containing the intersection point of the line in which the line segment lies with
							the plane and a boolean indicating whether or not it was (strictly) between the endpoints
	@throws java.lang.Error	If the line segment is parallel to the plane or the points are identical
	*/
	public static Pair<Vector3d,Boolean> determine_linesegment_intersection_with_plane(final Vector3d p1, final Vector3d p2,
																					  final Plane plane)
	{
		/*
		Derivation of the algorithm:

		The intersection of a line segment with a plane can be calculated by intersecting the line in which it
		lies with the plane and checking whether the intersection point is between the specified endpoints of
		the segment. The intersection point returned is (strictly) between the endpoints iff the value of the
		parameter t returned by determine_line_intersection_with_plane satisfies 0 < t < 1.
		*/

		// v = p2 - p1
		Vector3d v = VectorUtil.subtract(p2, p1);

		Pair<Vector3d,Double> intersection = determine_line_intersection_with_plane(p1, v, plane);
		return Pair.make_pair(intersection.first, 0 < intersection.second && intersection.second < 1);
	}

	/**
	Returns the square of the (nearest) distance of the specified point p from the line segment
	joining the two specified endpoints e1 and e2.

	<p><b>Preconditions:</b>
	<dl>
	<dd>The points e1 and e2 must not be equal
	</dl>

	@param p	The point whose distance squared from the line segment we want to determine
	@param e1	One endpoint of the line segment
	@param e2	The other endpoint
	@return		The square of the (nearest) distance of the point from the line segment
	@throws java.lang.Error	If e1 and e2 are equal
	*/
	public static double distance_squared_from_linesegment(final Vector2d p, final Vector2d e1, final Vector2d e2)
	{
		/*
		Derivation of the algorithm:

		The nearest distance is the perpendicular distance if the projection of p
		onto the line segment lies within its endpoints, or the distance between p
		and the nearest endpoint otherwise. By the projection of p onto the line
		segment, we mean the intersection of the line r = p + tn with the line
		segment, where n is the normal to the line in which the line segment lies.

		Well:
		The line in which the segment e1-e2 lies has the equation r = e1 + t(e2-e1).
		The normal n to that line is perpendicular to e2-e1, so if we say that e2-e1 = (a,b),
		then n = (b,-a).
		*/

		Vector2d n = new Vector2d(e2.y-e1.y, e1.x-e2.x);
		Pair<Vector2d,Boolean> intersect = determine_linesegment_intersection_with_line(e1, e2, p, n);

		if(intersect.second == true)
		{
			return p.distance_squared(intersect.first);
		}
		else
		{
			double d1 = p.distance_squared(e1), d2 = p.distance_squared(e2);
			return d1 < d2 ? d1 : d2;
		}
	}

	/**
	Returns an arbitrary unit vector perpendicular to the specified plane's normal.

	@param plane			The plane to which our arbitrary unit vector is to be coplanar
	@return					An arbitrary unit vector v such that v.dot(plane.get_normal()) == 0
	*/
	public static Vector3d generate_arbitrary_coplanar_unit_vector(final Plane plane)
	{
		Vector3d n = plane.get_normal();
		Vector3d up = new Vector3d(0,0,1);
		if(Math.abs(n.x) < EPSILON && Math.abs(n.y) < EPSILON)
		{
			// Special Case: n is too close to the vertical and hence n x up is roughly equal to (0,0,0)

			// Use a different vector instead of up (any different vector will do) and apply the same
			// method as in the else clause using the new vector.
			return n.cross(new Vector3d(1,0,0)).normalize();
		}
		else
		{
			// The normalized cross product of n and up satisfies the requirements of being
			// unit length and perpendicular to n (since we dealt with the special case where
			// n x up is zero, in all other cases it must be non-zero and we can normalize it
			// to give us a unit vector).
			return n.cross(up).normalize();
		}
	}

	/**
	Returns the plane in which the specified polygon lies.
	<p><b>Preconditions:</b>
	<dl>
	<dd>The polygon must be valid (it must have at least three vertices)
	<dd>The polygon's normal must be up-to-date
	</dl>

	@param poly		The polygon whose plane we want
	@return			...think about it...
	*/
	public static Plane make_plane(final Polygon poly)
	{
		Vector3d normal = poly.get_normal();
		double d = normal.dot(poly.get_vertices()[0]);
		return new Plane(normal, d);
	}

	/**
	Returns a polygon on the specified plane which is large enough to
	represent the plane itself for the purposes of the editor.

	@param plane	The plane for which we want to make a universe polygon
	@return			A polygon poly s.t. classify_polygon_against_plane(poly, plane) == CP_COPLANAR
					&& poly.get_normal().angle(plane.get_normal()) < EPSILON && poly is large enough
					to represent the plane itself for the purposes of the editor
	*/
	public static Polygon make_universe_polygon(final Plane plane)
	{
		final Vector3d origin = new Vector3d(0,0,0);
		Vector3d centre = nearest_point_in_plane(origin, plane);

		Vector3d[] planarVecs = new Vector3d[2];
		planarVecs[0] = generate_arbitrary_coplanar_unit_vector(plane);
		planarVecs[1] = planarVecs[0].cross(plane.get_normal()).normalize();		// obtain a perpendicular coplanar vector

		final double HALFSIDELENGTH = 1000000;	// something arbitrarily huge (but not too big, to avoid floating-point issues)
		for(int i=0; i<2; ++i) planarVecs[i].scale(HALFSIDELENGTH);

		Vector3d[] vertices = new Vector3d[4];
		for(int i=0; i<4; ++i) vertices[i] = centre.clone();
		vertices[0].subtract(planarVecs[0]);	vertices[0].subtract(planarVecs[1]);
		vertices[1].subtract(planarVecs[0]);	vertices[1].add(planarVecs[1]);
		vertices[2].add(planarVecs[0]);			vertices[2].add(planarVecs[1]);
		vertices[3].add(planarVecs[0]);			vertices[3].subtract(planarVecs[1]);

		return new Polygon(vertices);
	}

	/**
	Returns the point in the specified plane which is nearest to the specified point.

	@param p		The point whose nearest point in the plane we want to find
	@param plane	The plane
	@return			A point r s.t. classify_point_against_plane(r, plane) == CP_COPLANAR and there
					is no point r' s.t. classify_point_against_plane(r', plane) == CP_COPLANAR &&
					p.distance(r') < p.distance(r)
	*/
	public static Vector3d nearest_point_in_plane(final Vector3d p, final Plane plane)
	{
		/*
		Derivation of the algorithm:

		The nearest point in the plane is the point we get if we head from p towards the plane
		along the normal.
		*/

		double displacement = plane.displacement_to_point(p);
		Vector3d delta = plane.get_normal().clone();
		delta.scale(-displacement);
		Vector3d r = VectorUtil.add(p, delta);
		return r;
	}

	/**
	Returns the point membership classification of a point against a convex polygon.

	<p><b>Preconditions:</b>
	<dl>
	<dd>p != null
	<dd>poly != null
	<dd>The point p must lie in the plane of poly, i.e. classify_point_against_plane(p, make_plane(poly)) == CP_COPLANAR
	<dd>The polygon poly must be convex
	</dl>

	@param p	The point to classify
	@param poly The (convex) polygon against which to classify it
	@return		PMC_INSIDE, if the point is (strictly) inside the convex polygon, or PMC_SURFACE, if the point is on the edge
				of the polygon or at a vertex, or PMC_OUTSIDE, otherwise
	@throws java.lang.Error	If any of the preconditions are not met
	*/
	public static int point_in_convex_polygon(final Vector3d p, final Polygon poly)
	{
		// Check the preconditions.
		if(p == null || poly == null) throw new java.lang.Error();
		if(classify_point_against_plane(p, make_plane(poly)) != CP_COPLANAR) throw new java.lang.Error();
		// Note: Can't easily check convexity.

		// Main algorithm
		final Vector3d[] verts = poly.get_vertices();
		final int len = verts.length;

		// Check whether we're at a vertex.
		for(int i=0; i<len; ++i)
		{
			if(p.equals(verts[i])) return PMC_SURFACE;
		}

		// Calculate all the offset vectors. We do this here, rather than at the time of use,
		// because otherwise we'd have to calculate one of them twice.
		final Vector3d[] vecs = new Vector3d[len];
		for(int i=0; i<len; ++i)
		{
			vecs[i] = VectorUtil.subtract(verts[i], p);
		}

		// Sum the angles between consecutive offset vectors: if they sum to 2*PI, we're inside
		// the polygon, though we need to watch out for the point being on an edge, which would
		// give us an angle sum of 2*PI and hence PMC_INSIDE if we didn't explicitly check for it.
		double angleSum = 0;
		boolean edgeFlag = false;	// is the point on an edge of the polygon?
		for(int i=0; i<len; ++i)
		{
			int j = (i+1)%len;		// the next vertex in the (conceptually circular) vertex array
			double angle = VectorUtil.angle_between(vecs[i], vecs[j]);
			if(Math.abs(angle - Math.PI) < EPSILON) edgeFlag = true;
			angleSum += angle;
		}

		if(Math.abs(angleSum - 2*Math.PI) < EPSILON) return edgeFlag ? PMC_SURFACE : PMC_INSIDE;
		else return PMC_OUTSIDE;
	}

	//################## BEGIN HELPER METHODS FOR THE split_polygon METHOD ##################//
	/**
	Completes the original half of the split polygon (the side we did first). (It's used, obviously, in the
	split_polygon method.)

	@param originalHalf	The vertex list for the side of the split polygon we constructed first
	@param poly			The polygon we're splitting
	@param i			The index of the vertex we're currently processing
	@param startVert	The first vertex we processed
	*/
	private static void complete_original_half(LinkedList<Vector3d> originalHalf, final Polygon poly, int i, final int startVert)
	{
		final int vertCount = poly.get_vertices().length;

		while(i != startVert)
		{
			originalHalf.add(poly.get_vertices()[i].clone());
			i = next_vert(i, vertCount);
		}
	}

	/**
	Constructs one of the two halves of the split polygon. (It's used, obviously, in the split_polygon method.)

	<p><b>Preconditions:</b>
	<dl>
	<dd>cp == CP_BACK or CP_FRONT (initially, at any rate - it does get modified)
	<dd>currentHalf != otherHalf
	<dd>0 <= i < poly.get_vertices().length
	</dl>

	@param currentHalf	The vertices for the half of the polygon we're currently constructing
	@param otherHalf	The vertices for the other half of the polygon
	@param poly			The polygon we're splitting
	@param plane		The splitting plane
	@param i			The index of the vertex we're currently processing
	@param cp			The classification of that vertex against the splitting plane
	@return				A pair of integers, the first of which is the new value of i and
						the second which is the new value of cp
	*/
	private static Pair<Integer,Integer> construct_half_polygon(LinkedList<Vector3d> currentHalf,
																LinkedList<Vector3d> otherHalf,
																final Polygon poly, final Plane plane,
																int i, int cp)
	{
		final int vertCount = poly.get_vertices().length;

		int oldcp;	// stores the classification of the previous vertex against the plane (initially indeterminate)
		do
		{
			currentHalf.add(poly.get_vertices()[i].clone());
			i = next_vert(i, vertCount);
			oldcp = cp;
			cp = classify_point_against_plane(poly.get_vertices()[i], plane);
		} while(cp == oldcp);

		// If two consecutive points aren't on the same side of the plane and the first one
		// isn't coplanar, either the second one's coplanar or it's on the opposite side of
		// the plane from the first.
		if(cp == CP_COPLANAR)
		{
			currentHalf.add(poly.get_vertices()[i].clone());
			otherHalf.add(poly.get_vertices()[i].clone());
			i = next_vert(i, vertCount);
			cp = classify_point_against_plane(poly.get_vertices()[i], plane);

			// Having two coplanar points in sequence would imply that the polygon doesn't
			// straddle the plane, which violates our precondition.
			if(cp == CP_COPLANAR) throw new java.lang.Error();
		}
		else
		{
			Vector3d intersectionPoint = determine_linesegment_intersection_with_plane(poly.get_vertices()[prev_vert(i, vertCount)],
																					  poly.get_vertices()[i],
																					  plane).first;
			currentHalf.add(intersectionPoint.clone());
			otherHalf.add(intersectionPoint.clone());

			// Note: We could have used intersectionPoint for the first of these instead of its clone,
			// but it's safer to do it this way. The thinking behind that is that if the implementation
			// of determine_linesegment_intersection_with_plane is written in such a way that it might
			// return one of the original points as the answer (and not a clone of that point), then we
			// could end up with obscure, hard-to-track-down problems if we just blindly use the return
			// value.
		}

		// Note: At this point, the vertex with index i is in the other half of the polygon
		// (i.e. non-coplanar). Its classification against the plane is cp.
		return new Pair<Integer,Integer>(i, cp);
	}

	/**
	Returns the index of the next vertex (in winding order) after the one specified.

	@param index		Some vertex index
	@param vertCount	The number of vertices in the polygon
	@return				The index of the next vertex after the one specified
	*/
	private static int next_vert(int index, final int vertCount)
	{
		return (index+1)%vertCount;
	}

	/**
	Returns the index of the previous vertex (in winding order) before the one specified.

	@param index		Some vertex index
	@param vertCount	The number of vertices in the polygon
	@return				The index of the previous vertex before the one specified
	*/
	private static int prev_vert(int index, final int vertCount)
	{
		return (index+vertCount-1)%vertCount;
	}
	//################## END HELPER METHODS FOR THE split_polygon METHOD ##################//

	/**
	Splits a polygon across a plane.

	<p><b>Preconditions:</b>
	<dl>
	<dd>The polygon to split must be convex
	<dd>The polygon must straddle the plane
	</dl>

	@param poly				The polygon to split
	@param plane			The plane across which to split it
	@return					A pair of polygons, the first component of which is the polygon fragment
							in front of the plane and the second of which is that behind it
	@throws java.lang.Error	If the polygon doesn't straddle the plane
	*/
	public static Pair<Polygon,Polygon> split_polygon(final Polygon poly, final Plane plane)
	{
// FIXME: It's possible to check this during the algorithm and thus make polygon splitting slightly faster.
// Whether it's worth it depends on how much splitting we do. At the moment, this is more obviously right
// whence I'm leaving it this way for now.
		// Enforce the precondition about the polygon straddling the plane.
		if(classify_polygon_against_plane(poly, plane) != CP_STRADDLE) throw new java.lang.Error();

		LinkedList<Vector3d> frontHalf = new LinkedList<Vector3d>();
		LinkedList<Vector3d> backHalf = new LinkedList<Vector3d>();

		int startVert = 0;
		int cp = classify_point_against_plane(poly.get_vertices()[0], plane);
		if(cp == CP_COPLANAR)	// special case: we've started at the beginning of one half of the polygon
		{
			cp = classify_point_against_plane(poly.get_vertices()[1], plane);

			if(cp == CP_COPLANAR)
			{
				// If the polygon doesn't straddle the plane, we've violated a precondition, so throw an error.
				throw new java.lang.Error();
			}
			else
			{
				// Otherwise, start from the next vertex along to avoid writing extra special case code.
				startVert = 1;
			}
		}

		// At this point, cp contains the classification of poly.get_vertices()[startVert] against the plane
		// Note that cp != CP_COPLANAR

		switch(cp)
		{
			case CP_BACK:
			{
				Pair<Integer,Integer> p = construct_half_polygon(backHalf, frontHalf, poly, plane, startVert, cp);
				p = construct_half_polygon(frontHalf, backHalf, poly, plane, p.first, p.second);
				complete_original_half(backHalf, poly, p.first, startVert);
				break;
			}
			case CP_FRONT:
			{
				Pair<Integer,Integer> p = construct_half_polygon(frontHalf, backHalf, poly, plane, startVert, cp);
				p = construct_half_polygon(backHalf, frontHalf, poly, plane, p.first, p.second);
				complete_original_half(frontHalf, poly, p.first, startVert);
				break;
			}
		}

		// Construct the polygons from the lists of vertices. Note that we also need to copy the original
		// texture and texture plane across to the new polygons. It's obvious why the texture is the same,
		// if it's not obvious why the texture plane stays the same, simply consider that the new polygons
		// lie in the same plane as the old one.
		Pair<String,TexturePlane> textureDetails = poly.get_texture_details();
		Pair<Polygon,Polygon> ret = new Pair<Polygon,Polygon>();
		ret.first = new Polygon(frontHalf, textureDetails.first, textureDetails.second);
		ret.second = new Polygon(backHalf, textureDetails.first, textureDetails.second);

		return ret;
	}

	//################## TEST HARNESS ##################//
	public static class TestHarness extends TestHarnessAdapter
	{
		public void test_classify_point_against_plane()
		{
			Plane plane_a = new Plane(new Vector3d(1,0,0), 0);	// x = 0

			Vector3d vec_a = new Vector3d(1,23,9);
			int cp = classify_point_against_plane(vec_a, plane_a);
			output(cp == CP_FRONT, "true");

			Vector3d vec_b = new Vector3d(-1,7,8);
			cp = classify_point_against_plane(vec_b, plane_a);
			output(cp == CP_BACK, "true");

			Vector3d vec_c = new Vector3d(0,17,10);
			cp = classify_point_against_plane(vec_c, plane_a);
			output(cp == CP_COPLANAR, "true");

			// TESTME: I suppose more tests could be done here. I'm pretty sure it works though.
		}

		public void test_classify_polygon_against_plane()
		{
			Plane plane_a = new Plane(new Vector3d(1,0,0), 0);	// x = 0

			// Test a polygon which is strictly in front of the plane.
			Polygon poly_a = new Polygon(new Vector3d[] {new Vector3d(2,0,2), new Vector3d(2,0,0), new Vector3d(4,0,2)});
			int cp = classify_polygon_against_plane(poly_a, plane_a);
			output(cp == CP_FRONT, "true");

			// Test a polygon which is strictly behind the plane.
			Polygon poly_b = new Polygon(new Vector3d[] {new Vector3d(-4,0,2), new Vector3d(-4,0,0), new Vector3d(-2,0,2)});
			cp = classify_polygon_against_plane(poly_b, plane_a);
			output(cp == CP_BACK, "true");

			// Test a polygon which straddles the plane.
			Polygon poly_c = new Polygon(new Vector3d[] {new Vector3d(-2,0,2), new Vector3d(-2,0,0), new Vector3d(2,0,2)});
			cp = classify_polygon_against_plane(poly_c, plane_a);
			output(cp == CP_STRADDLE, "true");

			// Test a polygon which lies in the plane.
			Polygon poly_d = new Polygon(new Vector3d[] {new Vector3d(0,0,2), new Vector3d(0,0,0), new Vector3d(0,2,2)});
			cp = classify_polygon_against_plane(poly_d, plane_a);
			output(cp == CP_COPLANAR, "true");

			// Test a polygon which is in front of the plane, but touching it.
			Polygon poly_e = new Polygon(new Vector3d[] {new Vector3d(0,0,2), new Vector3d(0,0,0), new Vector3d(2,0,2)});
			cp = classify_polygon_against_plane(poly_e, plane_a);
			output(cp == CP_FRONT, "true");

			// Test a polygon which is behind the plane, but touching it.
			Polygon poly_f = new Polygon(new Vector3d[] {new Vector3d(-2,0,2), new Vector3d(-2,0,0), new Vector3d(0,0,2)});
			cp = classify_polygon_against_plane(poly_f, plane_a);
			output(cp == CP_BACK, "true");

			// TESTME: I suppose more tests could be done here. I'm pretty sure it works though.
		}

		public void test_construct_cuboid()
		{
			// TODO: Write some test code for this. Not hugely important, since it clearly works.
		}

		public void test_determine_line_intersection_with_plane()
		{
			// This method's very similar to determine_linesegment_intersection_with_plane,
			// but let's test it using different test cases - it's more interesting that way!

			// Let's test using a simple plane first.
			Plane plane_a = new Plane(new Vector3d(1,1,0), 5);		// x + y = 5

			output(determine_line_intersection_with_plane(new Vector3d(0,0,0), new Vector3d(1,1,0), plane_a), "((2.5, 2.5, 0.0), 2.5)");

			output(determine_line_intersection_with_plane(new Vector3d(0,0,0), new Vector3d(2,2,0), plane_a), "((2.5, 2.5, 0.0), 1.25)");

			output(determine_line_intersection_with_plane(new Vector3d(3,1,0), new Vector3d(0,5,0), plane_a), "((3.0, 2.0, 0.0), 0.2)");

			output(determine_line_intersection_with_plane(new Vector3d(3,3,0), new Vector3d(0,10,0), plane_a), "((3.0, 2.0, 0.0), -0.1)");

			// Line parallel to plane
			try
			{
				output(determine_line_intersection_with_plane(new Vector3d(0,0,0), new Vector3d(1,-1,0), plane_a), "Error");
			}
			catch(Error e)
			{
				output("", "");
			}

			// Invalid line - direction vector is zero
			try
			{
				output(determine_line_intersection_with_plane(new Vector3d(0,0,0), new Vector3d(0,0,0), plane_a), "Error");
			}
			catch(Error e)
			{
				output("", "");
			}

			// Now let's try a slightly more complicated plane.
			Plane plane_b = new Plane(new Vector3d(7,17,83), 23);	// 7x + 17y + 83z = 23

			// The numbers in this depend on each other in that (for example) 8 - 7.75 = 0.25, etc. This made it easier
			// to figure out the expected results of the test.
			output(determine_line_intersection_with_plane(new Vector3d(8,10,51), new Vector3d(-7.75,-8.75,-51), plane_b), "((0.25, 1.25, 0.0), 1.0)");

			// Line parallel to plane
			Vector3d vec_a = generate_arbitrary_coplanar_unit_vector(plane_b);
			try
			{
				output(determine_line_intersection_with_plane(new Vector3d(0,0,0), vec_a, plane_b), "Error");
			}
			catch(Error e)
			{
				output("", "");
			}

			// Invalid line - direction vector is zero
			try
			{
				output(determine_line_intersection_with_plane(new Vector3d(23,9,84), new Vector3d(0,0,0), plane_a), "Error");
			}
			catch(Error e)
			{
				output("", "");
			}

			// Test using the same direction vector twice. I got caught out by this before - we really don't want
			// to modify the direction vector we're passing in!
			Plane plane_c = new Plane(new Vector3d(1,0,0), 23);		// x = 23
			Vector3d vec_b = new Vector3d(1,0,0);

			output(determine_line_intersection_with_plane(new Vector3d(0,0,0), vec_b, plane_c), "((23.0, 0.0, 0.0), 23.0)");
			output(determine_line_intersection_with_plane(new Vector3d(0,1,0), vec_b, plane_c), "((23.0, 1.0, 0.0), 23.0)");

			// TESTME: I suppose more tests could be done here. I'm fairly sure it works though.
		}

		public void test_determine_linesegment_intersection_with_line()
		{
			// Test example 1 from the method documentation.
			Vector2d p1 = new Vector2d(0,0), p2 = new Vector2d(2,2), a = new Vector2d(1,-1);
			Vector2d v = new Vector2d(0,1);
			Pair<Vector2d,Boolean> intersect = determine_linesegment_intersection_with_line(p1, p2, a, v);
			output(intersect, "((1.0, 1.0), true)");

			// Test a limit case variation on it by shortening the line segment.
			p2 = new Vector2d(1,1);
			intersect = determine_linesegment_intersection_with_line(p1, p2, a, v);
			output(intersect, "((1.0, 1.0), false)");

			// Test example 2 from the method documentation.
			p1 = new Vector2d(1,0); p2 = new Vector2d(1,2); a = new Vector2d(0,0); v = new Vector2d(1,1);
			intersect = determine_linesegment_intersection_with_line(p1, p2, a, v);
			output(intersect, "((1.0, 1.0), true)");

			// Turn that one into a limit case variation as well.
			a = new Vector2d(0,1);
			intersect = determine_linesegment_intersection_with_line(p1, p2, a, v);
			output(intersect, "((1.0, 2.0), false)");

			// Let's test some cases which shouldn't work.

			// Test a case where the endpoints of the line segment are identical.
			p1 = new Vector2d(23,9); p2 = new Vector2d(23,9); a = new Vector2d(3,4); v = new Vector2d(5,6);
			try
			{
				intersect = determine_linesegment_intersection_with_line(p1, p2, a, v);
				output(intersect, "Error");
			}
			catch(Error e)
			{
				output("", "");
			}

			// Test a case where the line segment and the other line are parallel.
			p1 = new Vector2d(0,0); p2 = new Vector2d(3,5); a = new Vector2d(-1,0); v = new Vector2d(6,10);
			try
			{
				intersect = determine_linesegment_intersection_with_line(p1, p2, a, v);
				output(intersect, "Error");
			}
			catch(Error e)
			{
				output("", "");
			}

			// Test another parallel case, which happens to follow a different execution path before
			// throwing an error (this is an example of testing based on what we know about how the
			// code works - it's not black-box testing).
			p1 = new Vector2d(0,0); p2 = new Vector2d(0,17); a = new Vector2d(10,7); v = new Vector2d(0,8);
			try
			{
				intersect = determine_linesegment_intersection_with_line(p1, p2, a, v);
				output(intersect, "Error");
			}
			catch(Error e)
			{
				output("", "");
			}

			// TESTME: I suppose more tests could be done here. I'm fairly sure it works though.
		}

		public void test_determine_linesegment_intersection_with_plane()
		{
			Polygon poly_a = new Polygon(new Vector3d[] { new Vector3d(0,2,0), new Vector3d(2,2,0), new Vector3d(0,0,0) });
			Plane plane_a = make_plane(poly_a);
			Vector3d vec_a = new Vector3d(-1, -1, -1), vec_b = new Vector3d(1, 1, 1);
			Pair<Vector3d,Boolean> intersect_a = determine_linesegment_intersection_with_plane(vec_a, vec_b, plane_a);
			output(intersect_a, "((0.0, 0.0, 0.0), true)");

			vec_b.x = 0;
			vec_b.y = 0.5;
			intersect_a = determine_linesegment_intersection_with_plane(vec_b, vec_a, plane_a);
			output(intersect_a, "((-0.5, -0.25, 0.0), true)");

			Plane plane_b = new Plane(new Vector3d(1, 0, 1), 1);	// x + z - 1 = 0
			Vector3d vec_c = new Vector3d(0, 0, 0), vec_d = new Vector3d(1, 0, 1);
			intersect_a = determine_linesegment_intersection_with_plane(vec_c, vec_d, plane_b);
			output(intersect_a, "((0.5, 0.0, 0.5), true)");

			// Test a couple of limit cases, where one of the points is on the plane.
			Vector3d vec_e = new Vector3d(0.5, 0, 0.5);
			intersect_a = determine_linesegment_intersection_with_plane(vec_c, vec_e, plane_b);
			output(intersect_a, "((0.5, 0.0, 0.5), false)");
			intersect_a = determine_linesegment_intersection_with_plane(vec_d, vec_e, plane_b);
			output(intersect_a, "((0.5, 0.0, 0.5), false)");

			// Test the limit case where both points are on the plane.
			Vector3d vec_f = new Vector3d(1, 0, 0);
			try
			{
				intersect_a = determine_linesegment_intersection_with_plane(vec_e, vec_f, plane_b);
				output(intersect_a, "Error");
			}
			catch(Error e)
			{
				output("", "");
			}

			// Test the limit case where both points are equal.
			try
			{
				intersect_a = determine_linesegment_intersection_with_plane(vec_d, vec_d, plane_b);
				output(intersect_a, "Error");
			}
			catch(Error e)
			{
				output("", "");
			}

			// Test the limit case where both points are equal and also lie in the plane.
			try
			{
				intersect_a = determine_linesegment_intersection_with_plane(vec_f, vec_f, plane_b);
				output(intersect_a, "Error");
			}
			catch(Error e)
			{
				output("", "");
			}

			// Test another normal test case.
			Vector3d vec_g = new Vector3d(4, 0, -2);
			intersect_a = determine_linesegment_intersection_with_plane(vec_c, vec_g, plane_b);
			output(intersect_a, "((2.0, 0.0, -1.0), true)");

			// Test the limit case.
			Vector3d vec_h = new Vector3d(2, 0, -1);
			intersect_a = determine_linesegment_intersection_with_plane(vec_c, vec_h, plane_b);
			output(intersect_a, "((2.0, 0.0, -1.0), false)");

			// Test a case which is slightly beyond the limit case, just to make sure.
			Vector3d vec_m = new Vector3d(2.2, 0, -1.1);
			intersect_a = determine_linesegment_intersection_with_plane(vec_c, vec_m, plane_b);
			output(intersect_a, "((1.9999999999999998, 0.0, -0.9999999999999999), true)");	// it's close enough to (2,0,-1)!
		}

		public void test_distance_squared_from_linesegment()
		{
			// Test a few simple cases with a horizontal line segment.
			Vector2d p = new Vector2d(7,8), e1 = new Vector2d(0,0), e2 = new Vector2d(10,0);
			double distanceSquared = distance_squared_from_linesegment(p, e1, e2);
			output(distanceSquared, "64.0");

			p = new Vector2d(10,17);
			distanceSquared = distance_squared_from_linesegment(p, e1, e2);
			output(distanceSquared, "289.0");

			p = new Vector2d(0,23);
			distanceSquared = distance_squared_from_linesegment(p, e1, e2);
			output(distanceSquared, "529.0");

			p = new Vector2d(-9,-84);
			distanceSquared = distance_squared_from_linesegment(p, e1, e2);
			output(distanceSquared, "7137.0");

			p = new Vector2d(84,-9);
			distanceSquared = distance_squared_from_linesegment(p, e1, e2);
			output(distanceSquared, "5557.0");

			// Now test a few cases with a vertical line segment.
			e1 = new Vector2d(3,5); e2 = new Vector2d(3,7);

			p = new Vector2d(1,6);
			distanceSquared = distance_squared_from_linesegment(p, e1, e2);
			output(distanceSquared, "4.0");

			p = new Vector2d(3,6);
			distanceSquared = distance_squared_from_linesegment(p, e1, e2);
			output(distanceSquared, "0.0");

			p = new Vector2d(4,8);
			distanceSquared = distance_squared_from_linesegment(p, e1, e2);
			output(distanceSquared, "2.0");

			p = new Vector2d(2,3);
			distanceSquared = distance_squared_from_linesegment(p, e1, e2);
			output(distanceSquared, "5.0");

			// Finally, test a couple of cases with an "arbitrary" line segment.
			e1 = new Vector2d(7,8); e2 = new Vector2d(17,10);		// has direction vector (5k,k) for any non-zero k

			p = new Vector2d(12.2,8);	// not arbitrary, consider moving (0.2,-1) from midpoint of line segment
			distanceSquared = distance_squared_from_linesegment(p, e1, e2);
			// This is close enough to 1.04!
			output(distanceSquared, "1.0399999999999998");

			p = new Vector2d(6,8);
			distanceSquared = distance_squared_from_linesegment(p, e1, e2);
			output(distanceSquared, "1.0");

			// TESTME: I suppose more tests could be done here. I'm pretty sure it works though.
		}

		public void test_generate_arbitrary_coplanar_unit_vector()
		{
			// Test a special case where the normal is parallel to the up vector.
			Plane plane_a = new Plane(new Vector3d(0,0,2), 5);
			Vector3d vec_a = generate_arbitrary_coplanar_unit_vector(plane_a);
			boolean ok = Math.abs(vec_a.length() - 1) < EPSILON && vec_a.dot(plane_a.get_normal()) == 0;
			output(ok, "true");

			// Test a special case where the normal is parallel to the down vector (and thus also the up vector).
			Plane plane_b = new Plane(new Vector3d(0,0,-3), 7);
			Vector3d vec_b = generate_arbitrary_coplanar_unit_vector(plane_b);
			ok = Math.abs(vec_b.length() - 1) < EPSILON && vec_b.dot(plane_b.get_normal()) == 0;
			output(ok, "true");

			// Test a normal case.
			Plane plane_c = new Plane(new Vector3d(3,4,0), 8);
			Vector3d vec_c = generate_arbitrary_coplanar_unit_vector(plane_c);
			ok = Math.abs(vec_c.length() - 1) < EPSILON && vec_c.dot(plane_c.get_normal()) == 0;
			output(ok, "true");

			// Test another normal case.
			Plane plane_d = new Plane(new Vector3d(23,9,84), 51);
			Vector3d vec_d = generate_arbitrary_coplanar_unit_vector(plane_d);
			ok = Math.abs(vec_d.length() - 1) < EPSILON && vec_d.dot(plane_d.get_normal()) == 0;
			output(ok, "true");

			// TESTME: I suppose more tests could be done here. I'm pretty sure it works though.
		}

		public void test_make_plane()
		{
			Polygon poly_a = new Polygon(new Vector3d[] { new Vector3d(1,1,0), new Vector3d(0,1,1), new Vector3d(1,0,0) });
			Plane plane_a = make_plane(poly_a);
			output(plane_a, "(0.7071067811865476, 0.0, 0.7071067811865476, 0.7071067811865476)");

			Polygon poly_b = new Polygon(new Vector3d[] { new Vector3d(5.0/3.0,1,0), new Vector3d(0,1,5.0/4.0), new Vector3d(5.0/3.0,0,0) });
			Plane plane_b = make_plane(poly_b);
			output(plane_b, "(0.6, 0.0, 0.8, 1.0)");

			// Test a simple axis-aligned polygon
			Polygon poly_c = new Polygon(new Vector3d[] { new Vector3d(3,23,9), new Vector3d(3,7,8), new Vector3d(3,17,10) });
			Plane plane_c = make_plane(poly_c);
			output(plane_c, "(-1.0, -0.0, 0.0, -3.0)");

			// Test the same polygon, facing the other way
			Polygon poly_d = new Polygon(new Vector3d[] { new Vector3d(3,23,9), new Vector3d(3,17,10), new Vector3d(3,7,8) });
			Plane plane_d = make_plane(poly_d);
			output(plane_d, "(1.0, 0.0, 0.0, 3.0)");

			// TESTME: I suppose more tests could be done here. I'm pretty sure it works though.
		}

		public void test_make_universe_polygon()
		{
			// Test a case with a simple axis-aligned plane.
			Plane plane_a = new Plane(new Vector3d(1,0,0), 0);
			Polygon poly_a = make_universe_polygon(plane_a);
			boolean ok = classify_polygon_against_plane(poly_a, plane_a) == CP_COPLANAR &&
						 VectorUtil.angle_between(poly_a.get_normal(), plane_a.get_normal()) < EPSILON;
			output(ok, "true");

			// Test a case with the negation of the above plane.
			Plane plane_b = plane_a.negate();
			Polygon poly_b = make_universe_polygon(plane_b);
			ok = classify_polygon_against_plane(poly_b, plane_b) == CP_COPLANAR &&
				 VectorUtil.angle_between(poly_b.get_normal(), plane_b.get_normal()) < EPSILON;
			output(ok, "true");

			// Test a case with an arbitrary plane.
			Plane plane_c = new Plane(new Vector3d(0.6,0,0.8), 1);
			Polygon poly_c = make_universe_polygon(plane_c);
			ok = classify_polygon_against_plane(poly_c, plane_c) == CP_COPLANAR &&
				 VectorUtil.angle_between(poly_c.get_normal(), plane_c.get_normal()) < EPSILON;
			output(ok, "true");

			System.err.println(plane_c);
			System.err.println(poly_c);
			System.err.println(plane_c.get_normal());
			System.err.println(poly_c.get_normal());
			System.err.println(VectorUtil.angle_between(poly_c.get_normal(), plane_c.get_normal()));

			// TESTME: More tests would be helpful here. It would also be nice to test that
			// the universe polygons being returned are large enough, though exactly how we
			// do that remains to be seen.
		}

		public void test_nearest_point_in_plane()
		{
			// Test a case with an axis-aligned plane and a point in front of it.
			Plane plane_a = new Plane(new Vector3d(1,0,0), 0);
			Vector3d point_a = new Vector3d(23, 9, 84);
			Vector3d nearest_a = nearest_point_in_plane(point_a, plane_a);
			output(nearest_a, "(0.0, 9.0, 84.0)");

			// Test a case with a different axis-aligned plane and a point behind it.
			Plane plane_b = new Plane(new Vector3d(0,0,1), 17);
			Vector3d point_b = new Vector3d(-7,-8,-51);
			Vector3d nearest_b = nearest_point_in_plane(point_b, plane_b);
			output(nearest_b, "(-7.0, -8.0, 17.0)");

			// Test a case with the same plane and a coplanar point.
			Vector3d point_c = new Vector3d(51,10,17);
			Vector3d nearest_c = nearest_point_in_plane(point_c, plane_b);
			output(nearest_c, "(51.0, 10.0, 17.0)");

			// Test a case with a general plane that isn't axis-aligned (but is easy to verify).
			Plane plane_c = new Plane(new Vector3d(1,1,1), 0);
			Vector3d point_d = new Vector3d(-2,-2,-2);
			Vector3d nearest_d = nearest_point_in_plane(point_d, plane_c);
			// This is close enough to (0,0,0)!
			output(nearest_d, "(4.440892098500626E-16, 4.440892098500626E-16, 4.440892098500626E-16)");

			// TESTME: I suppose more tests could be done here. I'm pretty sure it works though.
		}

		public void test_point_in_convex_polygon()
		{
			// Test a simple case first, that of a point strictly inside a polygon.
			Polygon poly_a = new Polygon(new Vector3d[] { new Vector3d(0,0,0), new Vector3d(10,0,0), new Vector3d(10,10,0), new Vector3d(0,10,0) });
			Vector3d point_a = new Vector3d(5, 5, 0);
			output(PMC_String(point_in_convex_polygon(point_a, poly_a)), PMC_String(PMC_INSIDE));

			// Test another simple case, that of a point strictly outside the same polygon.
			Vector3d point_b = new Vector3d(15, 5, 0);
			output(PMC_String(point_in_convex_polygon(point_b, poly_a)), PMC_String(PMC_OUTSIDE));

			// Now it gets more interesting, let's test a point on the edge of the polygon (but not at a vertex).
			Vector3d point_c = new Vector3d(10, 5, 0);
			output(PMC_String(point_in_convex_polygon(point_c, poly_a)), PMC_String(PMC_SURFACE));

			// What about a point at a vertex?
			Vector3d point_d = new Vector3d(10, 10, 0);
			output(PMC_String(point_in_convex_polygon(point_d, poly_a)), PMC_String(PMC_SURFACE));

			// Let's try another polygon, "just in case".
			// A triangle on the plane x - y - 10 = 0
			Polygon poly_b = new Polygon(new Vector3d[] { new Vector3d(10,0,0), new Vector3d(20,10,10), new Vector3d(10,0,10) });

			Vector3d point_e = new Vector3d(11, 1, 5);
			output(PMC_String(point_in_convex_polygon(point_e, poly_b)), PMC_String(PMC_INSIDE));

			Vector3d point_f = new Vector3d(17, 7, 23);
			output(PMC_String(point_in_convex_polygon(point_f, poly_b)), PMC_String(PMC_OUTSIDE));

			Vector3d point_g = new Vector3d(11, 1, 1);
			output(PMC_String(point_in_convex_polygon(point_g, poly_b)), PMC_String(PMC_SURFACE));

			Vector3d point_h = new Vector3d(20, 10, 10);
			output(PMC_String(point_in_convex_polygon(point_h, poly_b)), PMC_String(PMC_SURFACE));

			// Let's try some error cases.

			// The point is null
			try
			{
				output(PMC_String(point_in_convex_polygon(null, poly_a)), "Error");
			}
			catch(Error e)
			{
				output("", "");
			}

			// The polygon is null
			try
			{
				output(PMC_String(point_in_convex_polygon(point_a, null)), "Error");
			}
			catch(Error e)
			{
				output("", "");
			}

			// The point isn't in the plane of the polygon
			try
			{
				output(PMC_String(point_in_convex_polygon(new Vector3d(5, 5, 5), poly_a)), "Error");
			}
			catch(Error e)
			{
				output("", "");
			}

			// TESTME: More tests might be useful.
		}

		public void test_split_polygon()
		{
			// Split a simple axis-aligned triangle across the y-z plane.
			Polygon poly_a = new Polygon(new Vector3d[] { new Vector3d(-1,0,0), new Vector3d(1,2,0), new Vector3d(-1,2,0) });
			Plane plane_a = new Plane(new Vector3d(1,0,0), 0);	// x = 0
			Pair<Polygon,Polygon> splitPair = split_polygon(poly_a, plane_a);
			output(splitPair, "([(0.0, 1.0, 0.0) (1.0, 2.0, 0.0) (0.0, 2.0, 0.0)], " +
							   "[(-1.0, 0.0, 0.0) (0.0, 1.0, 0.0) (0.0, 2.0, 0.0) (-1.0, 2.0, 0.0)])");

			// Split it across the negation of the plane (since it's easy to do and worth checking).
			splitPair = split_polygon(poly_a, plane_a.negate());
			output(splitPair, "([(-1.0, 0.0, 0.0) (0.0, 1.0, 0.0) (0.0, 2.0, 0.0) (-1.0, 2.0, 0.0)], " +
							   "[(0.0, 1.0, 0.0) (1.0, 2.0, 0.0) (0.0, 2.0, 0.0)])");

			// Let's make life a little more interesting by splitting a pentagon across the plane.
			// Since our splitting code worked slightly differently if we started the vertex list
			// at a different point, we'll check that. Furthermore, we'll have a point on the plane
			// this time to complicate matters.

			// Pick a random place to start
			Polygon poly_b = new Polygon(new Vector3d[] {	new Vector3d(-1,0,0), new Vector3d(1,0,0), new Vector3d(1,1,0),
															new Vector3d(0,2,0), new Vector3d(-1,1,0) });
			splitPair = split_polygon(poly_b, plane_a);
			output(splitPair, "([(0.0, 0.0, 0.0) (1.0, 0.0, 0.0) (1.0, 1.0, 0.0) (0.0, 2.0, 0.0)], " +
							   "[(-1.0, 0.0, 0.0) (0.0, 0.0, 0.0) (0.0, 2.0, 0.0) (-1.0, 1.0, 0.0)])");

			// Start with a different vertex
			Polygon poly_c = new Polygon(new Vector3d[] {	new Vector3d(-1,1,0), new Vector3d(-1,0,0), new Vector3d(1,0,0),
															new Vector3d(1,1,0), new Vector3d(0,2,0) });
			splitPair = split_polygon(poly_c, plane_a);
			output(splitPair, "([(0.0, 0.0, 0.0) (1.0, 0.0, 0.0) (1.0, 1.0, 0.0) (0.0, 2.0, 0.0)], " +
							   "[(-1.0, 1.0, 0.0) (-1.0, 0.0, 0.0) (0.0, 0.0, 0.0) (0.0, 2.0, 0.0)])");

			// Start with the coplanar vertex
			Polygon poly_d = new Polygon(new Vector3d[] {	new Vector3d(0,2,0), new Vector3d(-1,1,0), new Vector3d(-1,0,0),
															new Vector3d(1,0,0), new Vector3d(1,1,0) });
			splitPair = split_polygon(poly_d, plane_a);
			output(splitPair, "([(0.0, 0.0, 0.0) (1.0, 0.0, 0.0) (1.0, 1.0, 0.0) (0.0, 2.0, 0.0)], " +
							   "[(-1.0, 1.0, 0.0) (-1.0, 0.0, 0.0) (0.0, 0.0, 0.0) (0.0, 2.0, 0.0)])");

			// Start with a vertex on the other side of the plane
			Polygon poly_e = new Polygon(new Vector3d[] {	new Vector3d(1,0,0), new Vector3d(1,1,0), new Vector3d(0,2,0),
															new Vector3d(-1,1,0), new Vector3d(-1,0,0) });
			splitPair = split_polygon(poly_e, plane_a);
			output(splitPair, "([(1.0, 0.0, 0.0) (1.0, 1.0, 0.0) (0.0, 2.0, 0.0) (0.0, 0.0, 0.0)], " +
							   "[(0.0, 2.0, 0.0) (-1.0, 1.0, 0.0) (-1.0, 0.0, 0.0) (0.0, 0.0, 0.0)])");

			// Now let's test what happens if we have two coplanar vertices (note that we can't have more than that
			// if the polygon is convex and straddles the plane)
			Polygon poly_f = new Polygon(new Vector3d[] {	new Vector3d(0,0,0), new Vector3d(1,1,0), new Vector3d(1,2,0),
															new Vector3d(0,3,0), new Vector3d(-1,2,0), new Vector3d(-1,1,0) });
			splitPair = split_polygon(poly_f, plane_a);
			output(splitPair, "([(1.0, 1.0, 0.0) (1.0, 2.0, 0.0) (0.0, 3.0, 0.0) (0.0, 0.0, 0.0)], " +
							   "[(0.0, 3.0, 0.0) (-1.0, 2.0, 0.0) (-1.0, 1.0, 0.0) (0.0, 0.0, 0.0)])");

			// Finally, let's check some cases which shouldn't work

			// The polygon's on the plane
			try
			{
				Polygon poly_g = new Polygon(new Vector3d[] {	new Vector3d(0,0,0), new Vector3d(0,1,1), new Vector3d(0,0,1) });
				splitPair = split_polygon(poly_g, plane_a);
				output(splitPair, "Error");
			}
			catch(Error e)
			{
				output("", "");
			}

			// The polygon's in front of the plane
			try
			{
				Polygon poly_h = new Polygon(new Vector3d[] {	new Vector3d(1,0,0), new Vector3d(1,1,1), new Vector3d(1,0,1) });
				splitPair = split_polygon(poly_h, plane_a);
				output(splitPair, "Error");
			}
			catch(Error e)
			{
				output("", "");
			}

			// The polygon's behind the plane
			try
			{
				Polygon poly_h = new Polygon(new Vector3d[] {	new Vector3d(-1,0,0), new Vector3d(-1,1,1), new Vector3d(-1,0,1) });
				splitPair = split_polygon(poly_h, plane_a);
				output(splitPair, "Error");
			}
			catch(Error e)
			{
				output("", "");
			}

			// TESTME: More tests might be useful.
		}
	}

	public static void main(String[] args)
	{
		new TestHarness().run_tests();
	}
}