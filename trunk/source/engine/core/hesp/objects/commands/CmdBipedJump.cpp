/***
 * hesperus: CmdBipedJump.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmdBipedJump.h"

#include <hesp/bounds/BoundsManager.h>
#include <hesp/database/Database.h>
#include <hesp/nav/NavDataset.h>
#include <hesp/nav/NavManager.h>
#include <hesp/nav/NavMesh.h>
#include <hesp/objects/components/ICmpMovement.h>
#include <hesp/objects/components/ICmpSimulation.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
CmdBipedJump::CmdBipedJump(const ObjectID& objectID, const Vector3d& dir)
:	m_objectID(objectID), m_dir(dir)
{}

//#################### PUBLIC METHODS ####################
void CmdBipedJump::execute(const ObjectManager_Ptr& objectManager, int milliseconds)
{
	ICmpMovement_Ptr cmpMovement = objectManager->get_component(m_objectID, cmpMovement);			assert(cmpMovement != NULL);
	ICmpSimulation_Ptr cmpSimulation = objectManager->get_component(m_objectID, cmpSimulation);		assert(cmpSimulation != NULL);

	int mapIndex = objectManager->bounds_manager()->lookup_bounds_index(cmpSimulation->bounds_group(), cmpSimulation->posture());
	NavManager_CPtr navManager = objectManager->database()->get("db://NavManager", navManager);
	NavMesh_CPtr navMesh = navManager->dataset(mapIndex)->nav_mesh();

	if(cmpMovement->attempt_navmesh_acquisition(navMesh))
	{
		// FIXME: The jump strength should eventually be a property of the entity.
		const double JUMP_STRENGTH = 3;		// force of jump in Newtons
		Vector3d velocity = m_dir / JUMP_STRENGTH + Vector3d(0,0,1) * JUMP_STRENGTH;
		cmpSimulation->set_velocity(velocity);
	}
}

}
