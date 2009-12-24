/***
 * hesperus: AiGotoPositionBehaviour.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "AiGotoPositionBehaviour.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
AiGotoPositionBehaviour::AiGotoPositionBehaviour(const ObjectID& objectID, const ObjectManager *objectManager, const Vector3d& dest)
:	m_objectID(objectID), m_objectManager(objectManager), m_dest(dest)
{}

//#################### PUBLIC METHODS ####################
std::vector<ObjectCommand_Ptr> AiGotoPositionBehaviour::generate_commands(const SteeringPipeline_CPtr& steeringPipeline)
{
	// NYI
	throw 23;
}

}
