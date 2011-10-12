/***
 * hesperus: NavMeshUtil.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "NavMeshUtil.h"

#include <hesp/math/geom/GeomUtil.h>
#include <hesp/trees/OnionTree.h>
#include <hesp/trees/TreeUtil.h>
#include "NavMesh.h"
#include "NavPolygon.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
/**
Finds the nav polygon in which the specified point resides in the nav mesh, if any.

@param p					The point whose nav polygon we want to find
@param suggestedNavPoly		A suggestion for the result (generally speaking, for a moving object this would be the last nav polygon we were in)
@param polygons				The collision polygons for the level
@param tree					The onion tree for the level
@param navMesh				The nav mesh in which to find the nav polygon
@return						The index of the nav polygon in which the specified point resides, if any, or -1 otherwise
*/
int NavMeshUtil::find_nav_polygon(const Vector3d& p, int suggestedNavPoly, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_CPtr& tree,
								  const NavMesh_CPtr& navMesh)
{
	// It's good to be paranoid and do a range check: we might no longer be on the same navigation mesh, for instance.
	if(suggestedNavPoly >= static_cast<int>(navMesh->polygons().size())) suggestedNavPoly = -1;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Step 1:	If there's a suggested nav polygon, check it first.
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	int suggestedColPoly = -1;
	if(suggestedNavPoly != -1)
	{
		suggestedColPoly = navMesh->polygons()[suggestedNavPoly]->collision_poly_index();
		if(point_in_polygon(p, *polygons[suggestedColPoly])) return suggestedNavPoly;
	}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Step 2:	Find the other potential collision polygons in which the point could lie.
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	int leafIndex = TreeUtil::find_leaf_index(p, tree);
	const std::vector<int>& polyIndices = tree->leaf(leafIndex)->polygon_indices();

	std::vector<int> potentialColPolyIndices;
	for(std::vector<int>::const_iterator it=polyIndices.begin(), iend=polyIndices.end(); it!=iend; ++it)
	{
		// Note: We only add collision polygons which are also nav polygons to the list of potentials.
		if(*it != suggestedColPoly && navMesh->lookup_nav_poly_index(*it) != -1) potentialColPolyIndices.push_back(*it);
	}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Step 3:	Test each of the polygons to see whether the point's inside it, and return the index of the found nav polygon if so.
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	int potentialCount = static_cast<int>(potentialColPolyIndices.size());
	for(int i=0; i<potentialCount; ++i)
	{
		int colPolyIndex = potentialColPolyIndices[i];
		if(point_in_polygon(p, *polygons[colPolyIndex]))
		{
			int navPolyIndex = navMesh->lookup_nav_poly_index(colPolyIndex);
#if 0
			std::cout << "Now in polygon (" << colPolyIndex << ',' << navPolyIndex << ')' << std::endl;
#endif
			return navPolyIndex;
		}
	}

	// If we get here, the point was not in a nav polygon.
	if(suggestedNavPoly != -1)
	{
#if 0
		std::cout << "Lost navmesh at " << p << std::endl;
#endif
	}

	return -1;
}

}
