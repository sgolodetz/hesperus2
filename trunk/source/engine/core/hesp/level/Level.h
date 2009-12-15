/***
 * hesperus: Level.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LEVEL
#define H_HESP_LEVEL

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <hesp/portals/OnionPortal.h>
#include <hesp/portals/Portal.h>
#include <hesp/util/PolygonTypes.h>
#include <hesp/vis/VisTable.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class BSPTree> BSPTree_Ptr;
typedef shared_ptr<const class BSPTree> BSPTree_CPtr;
typedef shared_ptr<class GeometryRenderer> GeometryRenderer_Ptr;
typedef shared_ptr<const class GeometryRenderer> GeometryRenderer_CPtr;
class InputState;
typedef shared_ptr<class ModelManager> ModelManager_Ptr;
typedef shared_ptr<class NavManager> NavManager_Ptr;
typedef shared_ptr<const class NavManager> NavManager_CPtr;
typedef shared_ptr<class ObjectManager> ObjectManager_Ptr;
typedef shared_ptr<class OnionTree> OnionTree_Ptr;
typedef shared_ptr<const class OnionTree> OnionTree_CPtr;

class Level
{
	//#################### TYPEDEFS ####################
private:
	typedef std::vector<CollisionPolygon_Ptr> ColPolyVector;
	typedef shared_ptr<ColPolyVector> ColPolyVector_Ptr;
	typedef std::vector<OnionPortal_Ptr> OnionPortalVector;
	typedef std::vector<Portal_Ptr> PortalVector;

	//#################### PRIVATE VARIABLES ####################
private:
	GeometryRenderer_Ptr m_geomRenderer;
	BSPTree_Ptr m_tree;
	PortalVector m_portals;
	LeafVisTable_Ptr m_leafVis;
	ColPolyVector_Ptr m_onionPolygons;
	OnionTree_Ptr m_onionTree;
	OnionPortalVector m_onionPortals;
	NavManager_Ptr m_navManager;
	ObjectManager_Ptr m_objectManager;

	//#################### CONSTRUCTORS ####################
public:
	Level(const GeometryRenderer_Ptr& geomRenderer, const BSPTree_Ptr& tree,
		  const PortalVector& portals, const LeafVisTable_Ptr& leafVis,
		  const ColPolyVector_Ptr& onionPolygons, const OnionTree_Ptr& onionTree,
		  const OnionPortalVector& onionPortals, const NavManager_Ptr& navManager,
		  const ObjectManager_Ptr& objectManager);

	//#################### PUBLIC METHODS ####################
public:
	BSPTree_CPtr bsp_tree() const;
	std::vector<int> find_visible_leaves(const Vector3d& eye) const;
	GeometryRenderer_CPtr geom_renderer() const;
	NavManager_CPtr nav_manager() const;
	const ObjectManager_Ptr& object_manager();
	const ColPolyVector& onion_polygons() const;
	OnionTree_CPtr onion_tree() const;
	const PortalVector& portals() const;
	void update(int milliseconds, InputState& input);

	//#################### PRIVATE METHODS ####################
private:
	void do_activatables(InputState& input);
	void do_animations(int milliseconds);
	void do_gravity(int milliseconds);
	void do_physics(int milliseconds);
	void do_yokes(int milliseconds, InputState& input);
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Level> Level_Ptr;
typedef shared_ptr<const Level> Level_CPtr;

}

#endif
