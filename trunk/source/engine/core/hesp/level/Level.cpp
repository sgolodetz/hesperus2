/***
 * hesperus: Level.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Level.h"

#include <hesp/axes/NUVAxes.h>
#include <hesp/bounds/Bounds.h>
#include <hesp/bounds/BoundsManager.h>
#include <hesp/input/InputState.h>
#include <hesp/nav/NavDataset.h>
#include <hesp/nav/NavMesh.h>
#include <hesp/objects/base/ObjectCommand.h>
#include <hesp/objects/base/ObjectManager.h>
#include <hesp/objects/components/ICmpActivatable.h>
#include <hesp/objects/components/ICmpModelRender.h>
#include <hesp/objects/components/ICmpMovement.h>
#include <hesp/objects/components/ICmpOrientation.h>
#include <hesp/objects/components/ICmpSimulation.h>
#include <hesp/objects/components/ICmpYoke.h>
#include <hesp/objects/messages/MsgTimeElapsed.h>
#include <hesp/physics/PhysicsSystem.h>
#include <hesp/trees/BSPTree.h>
#include <hesp/trees/TreeUtil.h>

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

void Level::update(int milliseconds, InputState& input)
{
	do_yokes(milliseconds, input);
	do_physics(milliseconds);
	do_animations(milliseconds);
	do_activatables(input);

	// Safely create any new objects which were spawned during this update,
	// and destroy any objects which were queued up for destruction.
	m_objectManager->flush_queues();

	// Broadcast an elapsed time message so that time-sensitive components can update themselves.
	m_objectManager->broadcast_message(Message_CPtr(new MsgTimeElapsed(milliseconds)));
}

//#################### PRIVATE METHODS ####################
void Level::do_activatables(InputState& input)
{
	// Step 1:	Set all activatable objects to be unhighlighted when rendered.
	std::vector<ObjectID> activatables = m_objectManager->group("Activatables");
	for(size_t i=0, size=activatables.size(); i<size; ++i)
	{
		ICmpModelRender_Ptr cmpRender = m_objectManager->get_component(activatables[i], cmpRender);
		cmpRender->set_highlights(false);
	}

	// Step 2:	Find the half-ray representing the player's line of sight.
	ICmpOrientation_Ptr cmpPlayerOrientation = m_objectManager->get_component(m_objectManager->player(), cmpPlayerOrientation);
	ICmpPosition_Ptr cmpPlayerPosition = m_objectManager->get_component(m_objectManager->player(), cmpPlayerPosition);
	Vector3d eye = cmpPlayerPosition->position();
	Vector3d look = cmpPlayerOrientation->nuv_axes()->n();

	// Step 3:	Check all activatable objects in leaves in the PVS of the viewer's leaf and find the nearest within range (if any)
	//			whose AABB is intersected by the half-ray. (If there's no such object, return.)
	ObjectID nearestObject;
	double nearestDistSquared = INT_MAX;

	std::vector<int> visibleLeaves = find_visible_leaves(eye);
	for(size_t i=0, size=activatables.size(); i<size; ++i)
	{
		// TODO: Determine whether the activatable is in a visible leaf and skip further testing if not.

		ICmpSimulation_Ptr cmpSimulation = m_objectManager->get_component(activatables[i], cmpSimulation);

		const Bounds_CPtr& bounds = m_objectManager->bounds_manager()->bounds(cmpSimulation->bounds_group(), cmpSimulation->posture());
		const Vector3d& position = cmpSimulation->position();
		Vector3d localEye = eye - position;		// the eye location in local model coordinates
		boost::optional<std::pair<Vector3d,Vector3d> > result = bounds->determine_halfray_intersection(localEye, look);
		if(result)
		{
			Vector3d hit = result->first;		// the location of the hit (in local model coordinates)
			double distSquared = localEye.distance_squared(hit);
			if(distSquared < nearestDistSquared)
			{
				nearestObject = activatables[i];
				nearestDistSquared = distSquared;
			}
		}
	}

	const double RANGE = 3.0;
	if(!nearestObject.valid() || nearestDistSquared > RANGE*RANGE) return;

	// Step 4:	If the player is trying to activate an object, activate the nearest object. Otherwise, set it to be highlighted.
	if(input.mouse_button_down(MOUSE_BUTTON_RIGHT))
	{
		ICmpActivatable_Ptr cmpActivatable = m_objectManager->get_component(nearestObject, cmpActivatable);
		cmpActivatable->activated_by(m_objectManager->player());
		input.release_mouse_button(MOUSE_BUTTON_RIGHT);
	}
	else
	{
		ICmpModelRender_Ptr cmpRender = m_objectManager->get_component(nearestObject, cmpRender);
		cmpRender->set_highlights(true);
	}
}

void Level::do_animations(int milliseconds)
{
	// Update the model animations.
	std::vector<ObjectID> animatables = m_objectManager->group("Animatables");
	for(size_t i=0, size=animatables.size(); i<size; ++i)
	{
		ICmpModelRender_Ptr cmpRender = m_objectManager->get_component(animatables[i], cmpRender);
		cmpRender->update_animation(milliseconds, m_onionPolygons, m_onionTree, m_navManager);
	}
}

void Level::do_gravity(int milliseconds)
{
	// Apply gravity to moveable objects.

	// FIXME: Gravity strength should eventually be a level property.
	const double GRAVITY_STRENGTH = 9.81;	// strength of gravity in Newtons

	std::vector<ObjectID> moveables = m_objectManager->group("Moveables");
	for(size_t i=0, size=moveables.size(); i<size; ++i)
	{
		ICmpMovement_Ptr cmpMovement = m_objectManager->get_component(moveables[i], cmpMovement);
		ICmpSimulation_Ptr cmpSimulation = m_objectManager->get_component(moveables[i], cmpSimulation);
		Vector3d velocity = cmpSimulation->velocity();
		cmpSimulation->set_velocity(velocity + Vector3d(0,0,-GRAVITY_STRENGTH*(milliseconds/1000.0)));
		if(cmpMovement->single_move(cmpSimulation->velocity(), 7.0 /* FIXME */, milliseconds, m_onionTree))
		{
			// A collision occurred, so set the velocity back to zero.
			cmpSimulation->set_velocity(Vector3d(0,0,0));
		}
	}
}

void Level::do_physics(int milliseconds)
{
	// Treat gravity as a special case.
	do_gravity(milliseconds);

	m_objectManager->physics_system()->update(m_objectManager->bounds_manager(), m_onionTree, milliseconds);
}

void Level::do_yokes(int milliseconds, InputState& input)
{
	// Step 1:	Generate the desired object commands for the yokeable objects and add them to the queue.
	std::vector<ObjectID> yokeables = m_objectManager->group("Yokeables");

	std::list<ObjectCommand_Ptr> cmdQueue;	// implement the queue as a list so that we can use back_inserter below

	for(size_t i=0, size=yokeables.size(); i<size; ++i)
	{
		ICmpYoke_Ptr cmpYoke = m_objectManager->get_component(yokeables[i], cmpYoke);
		
		// Use the object's yoke component to generate object commands.
		std::vector<ObjectCommand_Ptr> commands = cmpYoke->generate_commands(input, m_onionPolygons, m_onionTree, m_navManager);

		// Append the object commands to the queue.
		std::copy(commands.begin(), commands.end(), std::back_inserter(cmdQueue));
	}

	// Step 2:	Execute the object commands.
	for(std::list<ObjectCommand_Ptr>::const_iterator it=cmdQueue.begin(), iend=cmdQueue.end(); it!=iend; ++it)
	{
		(*it)->execute(m_objectManager, m_onionPolygons, m_onionTree, m_navManager, milliseconds);
	}
}

}
