/***
 * hesperus: Level.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Level.h"

#include <hesp/level/nav/NavDataset.h>
#include <hesp/level/nav/NavMesh.h>
#include <hesp/level/trees/BSPTree.h>
#include <hesp/level/trees/TreeUtil.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
Level::Level(const GeometryRenderer_Ptr& geomRenderer, const BSPTree_Ptr& tree,
			 const PortalVector& portals, const LeafVisTable_Ptr& leafVis,
			 const ColPolyVector& onionPolygons, const OnionTree_Ptr& onionTree,
			 const OnionPortalVector& onionPortals, const NavManager_Ptr& navManager,
			 const ObjectManager_Ptr& objectManager)
:	m_geomRenderer(geomRenderer), m_tree(tree), m_portals(portals), m_leafVis(leafVis),
	m_onionPolygons(onionPolygons), m_onionTree(onionTree), m_onionPortals(onionPortals),
	m_navManager(navManager), m_objectManager(objectManager)
{}

//#################### PUBLIC METHODS ####################
BSPTree_CPtr Level::bsp_tree() const
{
	return m_tree;
}

/**
Determine which leaves are potentially visible from the specified eye position.
*/
std::vector<int> Level::find_visible_leaves(const Vector3d& eye) const
{
	bool allVisible = false;
	int curLeaf = TreeUtil::find_leaf_index(eye, m_tree);
	if(curLeaf >= m_tree->empty_leaf_count())
	{
		// If we're erroneously in a solid leaf, we assume all leaves are visible (the best we can do when rendering in this case is render the entire level).
		allVisible = true;
	}

	std::vector<int> visibleLeaves;
	for(int i=0, size=m_leafVis->size(); i<size; ++i)
	{
		// TODO: View frustum culling.
		if(allVisible || (*m_leafVis)(curLeaf,i)) visibleLeaves.push_back(i);
	}

	return visibleLeaves;
}

GeometryRenderer_CPtr Level::geom_renderer() const
{
	return m_geomRenderer;
}

NavManager_CPtr Level::nav_manager() const
{
	return m_navManager;
}

const ObjectManager_Ptr& Level::object_manager()
{
	return m_objectManager;
}

const std::vector<CollisionPolygon_Ptr>& Level::onion_polygons() const
{
	return m_onionPolygons;
}

OnionTree_CPtr Level::onion_tree() const
{
	return m_onionTree;
}

const std::vector<Portal_Ptr>& Level::portals() const
{
	return m_portals;
}

}
