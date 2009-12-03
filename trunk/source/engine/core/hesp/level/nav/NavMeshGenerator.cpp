/***
 * hesperus: NavMeshGenerator.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "NavMeshGenerator.h"

#include <hesp/math/Constants.h>
#include <hesp/math/Interval.h>
#include <hesp/math/geom/GeomUtil.h>
#include "NavMesh.h"
#include "NavPolygon.h"
#include "StepDownLink.h"
#include "StepUpLink.h"
#include "WalkLink.h"

namespace hesp {

//#################### LOCAL CLASSES ####################
class OrthonormalCoordSystem2D
{
	//#################### PRIVATE VARIABLES ####################
private:
	Vector3d m_o, m_n, m_u, m_v;

	//#################### CONSTRUCTORS ####################
public:
	OrthonormalCoordSystem2D(const Plane& plane)
	{
		Vector3d origin(0,0,0);
		m_o = nearest_point_in_plane(origin, plane);
		m_n = plane.normal();
		m_u = generate_specific_coplanar_unit_vector(plane);
		m_v = Vector3d(0,0,1);	// == m_u.cross(m_n).normalize()
	}

	//#################### PUBLIC METHODS ####################
public:
	Vector2d from_canonical(const Vector3d& x) const
	{
		return Vector2d((x-m_o).dot(m_u), (x-m_o).dot(m_v));
	}

	Vector3d to_canonical(const Vector2d& x) const
	{
		return m_o + x.x*m_u + x.y*m_v;
	}
};

//#################### CONSTRUCTORS ####################
/**
Constructs a NavMeshGenerator.

@param polygons				The input array of collision polygons
@param maxHeightDifference	The maximum distance that the character can step up/down (depends on the AABB map for which we're generating a navmesh)
*/
NavMeshGenerator::NavMeshGenerator(const ColPolyVector& polygons, double maxHeightDifference)
:	m_polygons(polygons), m_maxHeightDifference(maxHeightDifference), m_edgePlaneTable(UniquePlanePred(2 * PI/180, 0.005))
{
	int polyCount = static_cast<int>(polygons.size());

	// Construct the walkable polygon array.
	for(int i=0; i<polyCount; ++i)
	{
		if(m_polygons[i]->auxiliary_data().walkable())
		{
			m_walkablePolygons.push_back(NavPolygon_Ptr(new NavPolygon(i)));
		}
	}
}

//#################### PUBLIC METHODS ####################
NavMesh_Ptr NavMeshGenerator::generate_mesh()
{
	if(!m_mesh)
	{
		build_edge_plane_table();
		determine_links();
		m_mesh.reset(new NavMesh(m_walkablePolygons, m_navLinks));
		clean_intermediate();
	}
	return m_mesh;
}

//#################### PRIVATE METHODS ####################
void NavMeshGenerator::add_nav_link(const NavLink_Ptr& navLink)
{
	int linkIndex = static_cast<int>(m_navLinks.size());
	m_navLinks.push_back(navLink);
	m_walkablePolygons[navLink->source_poly()]->add_out_link(linkIndex);
	m_walkablePolygons[navLink->dest_poly()]->add_in_link(linkIndex);
}

/**
Builds the edge plane table. This tells us, for each (undirected) vertical
plane running through a nav polygon edge, which edges lie in it. We split
them into edges whose directed edge plane faces the same way as the undirected
one, and those whose directed edge plane faces the opposite way. The purpose
of this is to make sure we only look for links between nav polygons on either
side of the undirected plane.
*/
void NavMeshGenerator::build_edge_plane_table()
{
	const Vector3d up(0,0,1);

	int walkablePolyCount = static_cast<int>(m_walkablePolygons.size());

	for(int i=0; i<walkablePolyCount; ++i)
	{
		const CollisionPolygon& curPoly = *m_polygons[m_walkablePolygons[i]->collision_poly_index()];
		int vertCount = curPoly.vertex_count();
		for(int j=0; j<vertCount; ++j)
		{
			int k = (j+1)%vertCount;
			const Vector3d& p1 = curPoly.vertex(j);
			const Vector3d& p2 = curPoly.vertex(k);
			Plane_Ptr edgePlane = make_axial_plane(p1,p2,up);
			if(!edgePlane) throw Exception("Bad input to the navigation mesh generator: one of the polygons was vertical");
			Plane undirectedEdgePlane = edgePlane->to_undirected_form();
			bool sameFacing = edgePlane->normal().dot(undirectedEdgePlane.normal()) > 0;
			if(sameFacing)
				m_edgePlaneTable[undirectedEdgePlane].sameFacing.push_back(EdgeReference(i,j));
			else
				m_edgePlaneTable[undirectedEdgePlane].oppFacing.push_back(EdgeReference(i,j));
		}
	}
}

/**
Calculates the segments joining nav polygons at a link. For example, the walk link
between two polygons will contain the edge segment where they meet. Note that this
function implicitly calculates whether there is a link between two polygons, since
that is dependent on whether or not an appropriate edge segment can be found.

@param s1			One of the 2D endpoints of the source edge in the plane
@param s2			The other 2D endpoint of the source edge in the plane
@param d1			One of the 2D endpoints of the destination edge in the plane
@param d2			The other 2D endpoint of the destination edge in the plane
@param xOverlap		The horizontal overlap interval between the 2D edges in the plane
*/
NavMeshGenerator::LinkSegments
NavMeshGenerator::calculate_link_segments(const Vector2d& s1, const Vector2d& s2, const Vector2d& d1, const Vector2d& d2,
										  const Interval& xOverlap) const
{
	LinkSegments linkSegments;

	// Calculate the line equations yS = mS.x + cS and yD = mD.x + cD.
	assert(fabs(s2.x - s1.x) > EPSILON);
	assert(fabs(d2.x - d1.x) > EPSILON);

	double mS = (s2.y - s1.y) / (s2.x - s1.x);
	double mD = (d2.y - d1.y) / (d2.x - d1.x);
	double cS = s1.y - mS * s1.x;
	double cD = d1.y - mD * d1.x;

	double deltaM = mD - mS;
	double deltaC = cD - cS;

	if(fabs(deltaM) > EPSILON)
	{
		// If the gradients of the source and destination edges are different, then we get
		// a combination of step up/step down links.

		// We want to find:
		// (a) The point walkX at which yD = yS
		// (b) The point stepUpX at which yD - yS = MAX_HEIGHT_DIFFERENCE (this is the furthest point at which you can step up)
		// (c) The point stepDownX at which yS - yD = MAX_HEIGHT_DIFFERENCE (this is the furthest point at which you can step down)

		// (a) deltaM . walkX + deltaC = 0
		double walkX = -deltaC / deltaM;

		// (b) deltaM . stepUpX + deltaC = MAX_HEIGHT_DIFFERENCE
		double stepUpX = (m_maxHeightDifference - deltaC) / deltaM;

		// (c) deltaM . stepDownX + deltaC = -MAX_HEIGHT_DIFFERENCE
		double stepDownX = (-m_maxHeightDifference - deltaC) / deltaM;

		// Now construct the link intervals and clip them to the known x overlap interval.
		Interval stepDownInterval(std::min(walkX,stepDownX), std::max(walkX,stepDownX));
		Interval stepUpInterval(std::min(walkX,stepUpX), std::max(walkX,stepUpX));
		stepDownInterval = stepDownInterval.intersect(xOverlap);
		stepUpInterval = stepUpInterval.intersect(xOverlap);

		// Finally, construct the link segments from the link intervals.
		if(!stepDownInterval.empty())
		{
			Vector2d sL(stepDownInterval.low(), mS*stepDownInterval.low()+cS);
			Vector2d sH(stepDownInterval.high(), mS*stepDownInterval.high()+cS);
			linkSegments.stepDownSourceToDestSegment.reset(new LineSegment2d(sL,sH));

			Vector2d dL(stepDownInterval.low(), mD*stepDownInterval.low()+cD);
			Vector2d dH(stepDownInterval.high(), mD*stepDownInterval.high()+cD);
			linkSegments.stepUpDestToSourceSegment.reset(new LineSegment2d(dL,dH));
		}

		if(!stepUpInterval.empty())
		{
			Vector2d sL(stepUpInterval.low(), mS*stepUpInterval.low()+cS);
			Vector2d sH(stepUpInterval.high(), mS*stepUpInterval.high()+cS);
			linkSegments.stepUpSourceToDestSegment.reset(new LineSegment2d(sL,sH));

			Vector2d dL(stepUpInterval.low(), mD*stepUpInterval.low()+cD);
			Vector2d dH(stepUpInterval.high(), mD*stepUpInterval.high()+cD);
			linkSegments.stepDownDestToSourceSegment.reset(new LineSegment2d(dL,dH));
		}
	}
	else
	{
		// If the gradients of the source and destination edges are the same (i.e. the edges are parallel),
		// then we either get a step up/step down combination, or a walk link in either direction.
		if(fabs(deltaC) <= m_maxHeightDifference)
		{
			Vector2d s1(xOverlap.low(), mS*xOverlap.low()+cS);
			Vector2d s2(xOverlap.high(), mS*xOverlap.high()+cS);
			Vector2d d1(xOverlap.low(), mD*xOverlap.low()+cD);
			Vector2d d2(xOverlap.high(), mD*xOverlap.high()+cD);

			// There's a link between the lines, but we need to check the sign of deltaC to see which type.
			if(deltaC > SMALL_EPSILON)
			{
				// The destination is higher than the source: step up.
				linkSegments.stepUpSourceToDestSegment.reset(new LineSegment2d(s1,s2));
				linkSegments.stepDownDestToSourceSegment.reset(new LineSegment2d(d1,d2));
			}
			else if(deltaC < -SMALL_EPSILON)
			{
				// The destination is lower than the source: step down.
				linkSegments.stepDownSourceToDestSegment.reset(new LineSegment2d(s1,s2));
				linkSegments.stepUpDestToSourceSegment.reset(new LineSegment2d(d1,d2));
			}
			else	// |deltaC| < SMALL_EPSILON
			{
				// The destination and source are at the same level: just walk across.
				linkSegments.walkSegment.reset(new LineSegment2d(s1,s2));
			}
		}
	}

	return linkSegments;
}

/**
Clears all the intermediate data used when generating the mesh.
*/
void NavMeshGenerator::clean_intermediate()
{
	m_edgePlaneTable.clear();
	m_navLinks.swap(NavLinkVector());
}

void NavMeshGenerator::determine_links()
{
	for(EdgePlaneTable::const_iterator it=m_edgePlaneTable.begin(), iend=m_edgePlaneTable.end(); it!=iend; ++it)
	{
		// Generate (n,u,v) coordinate system for plane, where v = (0,0,1).
		const Plane& plane = it->first;
		OrthonormalCoordSystem2D coordSystem(plane);

		// Check pairs of different-facing edges to see whether we need to create any links.
		const EdgeReferences& sameFacingEdgeRefs = it->second.sameFacing;
		const EdgeReferences& oppFacingEdgeRefs = it->second.oppFacing;
		int sameFacingEdgeRefCount = static_cast<int>(sameFacingEdgeRefs.size());
		int oppFacingEdgeRefCount = static_cast<int>(oppFacingEdgeRefs.size());

		for(int j=0; j<sameFacingEdgeRefCount; ++j)
		{
			const EdgeReference& edgeJ = sameFacingEdgeRefs[j];
			NavPolygon& navPolyJ = *m_walkablePolygons[edgeJ.navPolyIndex];
			const CollisionPolygon& colPolyJ = *m_polygons[navPolyJ.collision_poly_index()];
			int mapIndexJ = colPolyJ.auxiliary_data().map_index();

			// Calculate the 2D coordinates of edgeJ in the plane.
			const Vector3d& p1J = colPolyJ.vertex(edgeJ.startVertex);
			const Vector3d& p2J = colPolyJ.vertex((edgeJ.startVertex+1) % colPolyJ.vertex_count());
			Vector2d q1J = coordSystem.from_canonical(p1J), q2J = coordSystem.from_canonical(p2J);
			Interval xIntervalJ(std::min(q1J.x,q2J.x), std::max(q1J.x,q2J.x));

			for(int k=0; k<oppFacingEdgeRefCount; ++k)
			{
				const EdgeReference& edgeK = oppFacingEdgeRefs[k];
				NavPolygon& navPolyK = *m_walkablePolygons[edgeK.navPolyIndex];
				const CollisionPolygon& colPolyK = *m_polygons[navPolyK.collision_poly_index()];
				int mapIndexK = colPolyK.auxiliary_data().map_index();

				// We only want to create links between polygons in the same map.
				if(mapIndexJ != mapIndexK) continue;

				// Calculate the 2D coordinates of edgeK in the plane.
				const Vector3d& p1K = colPolyK.vertex(edgeK.startVertex);
				const Vector3d& p2K = colPolyK.vertex((edgeK.startVertex+1) % colPolyK.vertex_count());
				Vector2d q1K = coordSystem.from_canonical(p1K), q2K = coordSystem.from_canonical(p2K);

				// Calculate the x overlap between the 2D edges. If there's no overlap,
				// then we don't need to carry on looking for a link.
				Interval xIntervalK(std::min(q1K.x,q2K.x), std::max(q1K.x,q2K.x));
				Interval xOverlap = xIntervalJ.intersect(xIntervalK);
				if(xOverlap.empty()) continue;

				// Calculate the segments for the various types of link.
				LinkSegments linkSegments = calculate_link_segments(q1J, q2J, q1K, q2K, xOverlap);

				// Add the appropriate links.
				if(linkSegments.stepDownSourceToDestSegment)
				{
					assert(linkSegments.stepUpDestToSourceSegment != NULL);

					// Add a step down link from j -> k, and a step up one from k -> j.
					Vector3d j1 = coordSystem.to_canonical(linkSegments.stepDownSourceToDestSegment->e1);
					Vector3d j2 = coordSystem.to_canonical(linkSegments.stepDownSourceToDestSegment->e2);
					Vector3d k1 = coordSystem.to_canonical(linkSegments.stepUpDestToSourceSegment->e1);
					Vector3d k2 = coordSystem.to_canonical(linkSegments.stepUpDestToSourceSegment->e2);
					add_nav_link(NavLink_Ptr(new StepDownLink(edgeJ.navPolyIndex, edgeK.navPolyIndex, j1, j2, k1, k2)));
					add_nav_link(NavLink_Ptr(new StepUpLink(edgeK.navPolyIndex, edgeJ.navPolyIndex, k1, k2, j1, j2)));
				}
				if(linkSegments.stepUpSourceToDestSegment)
				{
					assert(linkSegments.stepDownDestToSourceSegment != NULL);

					// Add a step up link from j -> k, and a step down one from k -> j.
					Vector3d j1 = coordSystem.to_canonical(linkSegments.stepUpSourceToDestSegment->e1);
					Vector3d j2 = coordSystem.to_canonical(linkSegments.stepUpSourceToDestSegment->e2);
					Vector3d k1 = coordSystem.to_canonical(linkSegments.stepDownDestToSourceSegment->e1);
					Vector3d k2 = coordSystem.to_canonical(linkSegments.stepDownDestToSourceSegment->e2);
					add_nav_link(NavLink_Ptr(new StepUpLink(edgeJ.navPolyIndex, edgeK.navPolyIndex, j1, j2, k1, k2)));
					add_nav_link(NavLink_Ptr(new StepDownLink(edgeK.navPolyIndex, edgeJ.navPolyIndex, k1, k2, j1, j2)));
				}
				if(linkSegments.walkSegment)
				{
					// Add a walk link from j -> k, and one from k -> j.
					Vector3d e1 = coordSystem.to_canonical(linkSegments.walkSegment->e1);
					Vector3d e2 = coordSystem.to_canonical(linkSegments.walkSegment->e2);
					add_nav_link(NavLink_Ptr(new WalkLink(edgeJ.navPolyIndex, edgeK.navPolyIndex, e1, e2)));
					add_nav_link(NavLink_Ptr(new WalkLink(edgeK.navPolyIndex, edgeJ.navPolyIndex, e1, e2)));
				}
			}
		}
	}
}

}
