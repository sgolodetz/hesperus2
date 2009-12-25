/***
 * hesperus: AiBipedMoveToPositionBehaviour.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "AiBipedMoveToPositionBehaviour.h"

#include "AiSequenceBehaviour.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
AiBipedMoveToPositionBehaviour::AiBipedMoveToPositionBehaviour(const ObjectID& objectID, const ObjectManager *objectManager, const Vector3d& dest)
:	m_objectID(objectID), m_objectManager(objectManager), m_dest(dest)
{}

//#################### PUBLIC METHODS ####################
std::vector<ObjectCommand_Ptr> AiBipedMoveToPositionBehaviour::generate_commands()
{
	if(!m_plan) make_plan();
	return m_plan->generate_commands();
}

AiBehaviour::Status AiBipedMoveToPositionBehaviour::status() const
{
	if(m_plan) return m_plan->status();
	else return UNFINISHED;
}

//#################### PRIVATE METHODS ####################
void AiBipedMoveToPositionBehaviour::make_plan()
{
	// NYI
	throw 23;
}

}
