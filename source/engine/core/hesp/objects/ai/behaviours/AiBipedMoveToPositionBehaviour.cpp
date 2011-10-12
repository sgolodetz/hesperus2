/***
 * hesperus: AiBipedMoveToPositionBehaviour.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "AiBipedMoveToPositionBehaviour.h"

#include <cassert>

#include <hesp/bounds/BoundsManager.h>
#include <hesp/database/Database.h>
#include <hesp/nav/GlobalPathfinder.h>
#include <hesp/nav/NavDataset.h>
#include <hesp/nav/NavManager.h>
#include <hesp/nav/NavMeshUtil.h>
#include <hesp/objects/components/ICmpMovement.h>
#include <hesp/objects/components/ICmpSimulation.h>
#include <hesp/trees/OnionTree.h>
#include <hesp/util/PolygonTypes.h>
#include "AiSequenceBehaviour.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
AiBipedMoveToPositionBehaviour::AiBipedMoveToPositionBehaviour(const ObjectID& objectID, const ObjectManager *objectManager, const Vector3d& dest)
:	m_objectID(objectID), m_objectManager(objectManager), m_dest(dest), m_status(UNFINISHED)
{}

//#################### PUBLIC METHODS ####################
std::vector<ObjectCommand_Ptr> AiBipedMoveToPositionBehaviour::generate_commands()
{
	if(!m_plan) make_plan();

	// Note: It's possible that a suitable plan could not be generated (e.g. if the object's currently in mid-air).
	if(m_plan) return m_plan->generate_commands();
	else return std::vector<ObjectCommand_Ptr>();
}

AiBehaviour::Status AiBipedMoveToPositionBehaviour::status() const
{
	if(m_status != UNFINISHED) return m_status;
	else if(m_plan) return m_plan->status();
	else return UNFINISHED;
}

//#################### PRIVATE METHODS ####################
void AiBipedMoveToPositionBehaviour::make_plan()
{
	const Database& db = *m_objectManager->database();
	NavManager_CPtr navManager = db.get("db://NavManager", navManager);
	shared_ptr<std::vector<CollisionPolygon_Ptr> > polygons = db.get("db://OnionPolygons", polygons);
	OnionTree_CPtr tree = db.get("db://OnionTree", tree);

	ICmpMovement_CPtr cmpMovement = m_objectManager->get_component(m_objectID, cmpMovement);		assert(cmpMovement != NULL);
	ICmpSimulation_CPtr cmpSimulation = m_objectManager->get_component(m_objectID, cmpSimulation);	assert(cmpSimulation != NULL);

	const Vector3d& source = cmpSimulation->position();

	if(cmpMovement->traversing_link())
	{
		// TODO: This relies on being able to pathfind from the middle of a nav link.
		m_status = FAILED;
		return;
	}
	else
	{
		int mapIndex = m_objectManager->bounds_manager()->lookup_bounds_index(cmpSimulation->bounds_group(), cmpSimulation->posture());
		NavDataset_CPtr navDataset = navManager->dataset(mapIndex);
		NavMesh_CPtr navMesh = navDataset->nav_mesh();
		GlobalPathfinder pathfinder(navMesh, navDataset->adjacency_list(), navDataset->path_table());

		int suggestedSourcePoly = cmpMovement->cur_nav_poly_index();
		int sourcePoly = NavMeshUtil::find_nav_polygon(source, suggestedSourcePoly, *polygons, tree, navMesh);
		if(sourcePoly == -1)	{ m_status = FAILED; return; }
		int destPoly = NavMeshUtil::find_nav_polygon(m_dest, -1, *polygons, tree, navMesh);
		if(destPoly == -1)		{ m_status = FAILED; return; }

		std::list<int> path;
		bool pathFound = pathfinder.find_path(source, sourcePoly, m_dest, destPoly, path);
		if(!pathFound)			{ m_status = FAILED; return; }

		// NYI
		throw 23;
	}
}

}
