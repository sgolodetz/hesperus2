/***
 * hesperus: CmpBipedAnimChooser.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpBipedAnimChooser.h"

#include <hesp/bounds/BoundsManager.h>
#include <hesp/database/Database.h>
#include <hesp/nav/NavDataset.h>
#include <hesp/nav/NavManager.h>
#include <hesp/util/Properties.h>
#include "ICmpInventory.h"
#include "ICmpMovement.h"
#include "ICmpOwnable.h"
#include "ICmpSimulation.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpBipedAnimChooser::CmpBipedAnimChooser()
{
	reset_flags();
}

//#################### STATIC FACTORY METHODS ####################
IObjectComponent_Ptr CmpBipedAnimChooser::load(const Properties&)
{
	return IObjectComponent_Ptr(new CmpBipedAnimChooser);
}

//#################### PUBLIC METHODS ####################
void CmpBipedAnimChooser::check_dependencies() const
{
	check_dependency<ICmpHealth>();
	check_dependency<ICmpMovement>();
	check_dependency<ICmpSimulation>();
}

std::string CmpBipedAnimChooser::choose_animation()
{
	// Determine the biped's alive/dying/dead status. If dead, early-out.
	ICmpHealth::HealthStatus healthStatus = determine_health_status();
	if(healthStatus == ICmpHealth::DEAD) return "dead";

	// If the biped's not yet dead, determine its movement type.
	MovementType movementType = determine_movement_type();

	// Determine whether or not the biped is crouching.
	bool crouching = determine_crouching();

	// Lookup the current animation extension (if any).
	std::string animExtension = determine_anim_extension();

	// Choose the animation based on movement type, health status, crouch status and animation extension.
	std::string animationName;

	if(healthStatus == ICmpHealth::ALIVE)
	{
		switch(movementType)
		{
			case AIR:	animationName = "air";	break;
			case IDLE:	animationName = "idle";	break;
			case RUN:	animationName = "run";	break;
			case WALK:	animationName = "walk";	break;
			default:	throw Exception("Unexpected movement type");
		}
	}
	else
	{
		if(movementType == AIR)	animationName = "dieair";
		else					animationName = "die";
	}

	if(crouching)			animationName += "crouch";
	if(animExtension != "")	animationName += "_" + animExtension;

	// Reset any flags to false ready for the next frame.
	reset_flags();

	return animationName;
}

Properties CmpBipedAnimChooser::save() const
{
	return Properties();
}

void CmpBipedAnimChooser::set_run_flag()
{
	m_runFlag = true;
}

void CmpBipedAnimChooser::set_walk_flag()
{
	m_walkFlag = true;
}

//#################### PRIVATE METHODS ####################
std::string CmpBipedAnimChooser::determine_anim_extension() const
{
	std::string animExtension;
	ICmpInventory_Ptr cmpInventory = m_objectManager->get_component(m_objectID, cmpInventory);
	if(cmpInventory)
	{
		ObjectID activeItem = cmpInventory->active_item();
		if(activeItem.valid())
		{
			ICmpOwnable_Ptr cmpItemOwnable = m_objectManager->get_component(activeItem, cmpItemOwnable);	assert(cmpItemOwnable);
			animExtension = cmpItemOwnable->anim_extension();
		}
	}
	return animExtension;
}

bool CmpBipedAnimChooser::determine_crouching() const
{
	ICmpSimulation_Ptr cmpSimulation = m_objectManager->get_component(m_objectID, cmpSimulation);	assert(cmpSimulation != NULL);
	return cmpSimulation->posture() == "crouch";
}

ICmpHealth::HealthStatus CmpBipedAnimChooser::determine_health_status() const
{
	ICmpHealth_Ptr cmpHealth = m_objectManager->get_component(m_objectID, cmpHealth);	assert(cmpHealth != NULL);
	return cmpHealth->status();
}

CmpBipedAnimChooser::MovementType CmpBipedAnimChooser::determine_movement_type()
{
	MovementType movementType = UNKNOWN;

	// Determine whether or not the biped's in the air.
	ICmpMovement_Ptr cmpMovement = m_objectManager->get_component(m_objectID, cmpMovement);			assert(cmpMovement != NULL);
	ICmpSimulation_CPtr cmpSimulation = m_objectManager->get_component(m_objectID, cmpSimulation);	assert(cmpSimulation != NULL);
	int mapIndex = m_objectManager->bounds_manager()->lookup_bounds_index(cmpSimulation->bounds_group(), cmpSimulation->posture());
	NavManager_CPtr navManager = m_objectManager->database()->get("db://NavManager", navManager);
	if(!cmpMovement->attempt_navmesh_acquisition(navManager->dataset(mapIndex)->nav_mesh()))
	{
		movementType = AIR;
	}
	else
	{
		// If it's not in the air, work out its actual movement type from the flags.
		if(m_runFlag)		movementType = RUN;
		else if(m_walkFlag)	movementType = WALK;
		else				movementType = IDLE;
	}

	return movementType;
}

void CmpBipedAnimChooser::reset_flags()
{
	m_runFlag = m_walkFlag = false;
}

}
