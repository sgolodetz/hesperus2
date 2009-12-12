/***
 * hesperus: NavMeshGenerator.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_NAVMESHGENERATOR
#define H_HESP_NAVMESHGENERATOR

#include <map>

#include <hesp/math/geom/LineSegment.h>
#include <hesp/math/geom/UniquePlanePred.h>
#include <hesp/math/vectors/Vector2d.h>
#include <hesp/util/PolygonTypes.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
class Interval;
typedef shared_ptr<class NavLink> NavLink_Ptr;
typedef shared_ptr<class NavMesh> NavMesh_Ptr;
typedef shared_ptr<class NavPolygon> NavPolygon_Ptr;

class NavMeshGenerator
{
	//#################### NESTED CLASSES ####################
private:
	struct EdgeReference
	{
		int navPolyIndex;	// the index of the nav polygon in which this edge resides
		int startVertex;

		EdgeReference(int navPolyIndex_, int startVertex_) : navPolyIndex(navPolyIndex_), startVertex(startVertex_) {}
	};

	typedef std::vector<EdgeReference> EdgeReferences;

	struct EdgeReferencesPair
	{
		EdgeReferences sameFacing;	// the edge planes for these edges face the same way as the undirected edge planes
		EdgeReferences oppFacing;	// the edge planes for these edges face the opposite way to the undirected edge planes
	};

	struct LinkSegments
	{
		// TODO: We can add jump down and jump up segments here if we want.
		LineSegment2d_Ptr stepDownSourceToDestSegment;
		LineSegment2d_Ptr stepDownDestToSourceSegment;
		LineSegment2d_Ptr stepUpSourceToDestSegment;
		LineSegment2d_Ptr stepUpDestToSourceSegment;
		LineSegment2d_Ptr walkSegment;
	};

	//#################### TYPEDEFS ####################
private:
	typedef std::vector<CollisionPolygon_Ptr> ColPolyVector;
	typedef std::vector<NavPolygon_Ptr> NavPolyVector;
	typedef std::map<Plane,EdgeReferencesPair,UniquePlanePred> EdgePlaneTable;
	typedef std::vector<NavLink_Ptr> NavLinkVector;

	//#################### PRIVATE VARIABLES ####################
private:
	// Input data
	ColPolyVector m_polygons;
	NavPolyVector m_walkablePolygons;
	double m_maxHeightDifference;

	// Intermediate data
	EdgePlaneTable m_edgePlaneTable;
	NavLinkVector m_navLinks;

	// Output data
	NavMesh_Ptr m_mesh;

	//#################### CONSTRUCTORS ####################
public:
	NavMeshGenerator(const ColPolyVector& polygons, double maxHeightDifference);

	//#################### PUBLIC METHODS ####################
public:
	NavMesh_Ptr generate_mesh();

	//#################### PRIVATE METHODS ####################
private:
	void add_nav_link(const NavLink_Ptr& navLink);
	void build_edge_plane_table();
	LinkSegments calculate_link_segments(const Vector2d& s1, const Vector2d& s2, const Vector2d& d1, const Vector2d& d2, const Interval& xOverlap) const;
	void clean_intermediate();
	void determine_links();
};

}

#endif
