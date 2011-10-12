/***
 * hesperus: AiBipedMoveTowardsBehaviour.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "AiBipedMoveTowardsBehaviour.h"

#include <hesp/math/Constants.h>
#include <hesp/objects/base/ObjectManager.h>
#include <hesp/objects/components/ICmpSimulation.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
AiBipedMoveTowardsBehaviour::AiBipedMoveTowardsBehaviour(const ObjectID& objectID, const ObjectManager *objectManager, const Vector3d& dest)
:	m_objectID(objectID), m_objectManager(objectManager), m_dest(dest)
{}

//#################### PUBLIC METHODS ####################
std::vector<ObjectCommand_Ptr> AiBipedMoveTowardsBehaviour::generate_commands()
{
	// Calculate the direction in which to move.
	ICmpSimulation_CPtr cmpSimulation = m_objectManager->get_component(m_objectID, cmpSimulation);	assert(cmpSimulation != NULL);
	const Vector3d& source = cmpSimulation->position();
	Vector3d dir = m_dest - source;
	double len = dir.length();
	if(len < SMALL_EPSILON) return std::vector<ObjectCommand_Ptr>();
	else dir /= len;

	// Generate the appropriate command by delegating to the subclass, and return it.
	std::vector<ObjectCommand_Ptr> ret;
	ret.push_back(generate_command(m_objectID, dir));
	return ret;
}

AiBehaviour::Status AiBipedMoveTowardsBehaviour::status() const
{
	// TODO:	This could be improved by checking whether we're making progress towards the destination or not.
	//			If not, it should return FAILED to facilitate a change of plan.

	// Check whether the distance to the destination is less than or equal to the success radius.
	ICmpSimulation_CPtr cmpSimulation = m_objectManager->get_component(m_objectID, cmpSimulation);	assert(cmpSimulation != NULL);
	const Vector3d& source = cmpSimulation->position();
	double distance = (m_dest - source).length();
	if(distance <= success_radius()) return SUCCEEDED;
	else return UNFINISHED;
}

}
