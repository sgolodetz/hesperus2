/***
 * hesperus: AiBipedGotoPositionBehaviour.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "AiBipedGotoPositionBehaviour.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
AiBipedGotoPositionBehaviour::AiBipedGotoPositionBehaviour(const ObjectID& objectID, const ObjectManager *objectManager, const Vector3d& dest)
:	m_objectID(objectID), m_objectManager(objectManager), m_dest(dest)
{}

//#################### PUBLIC METHODS ####################
std::vector<ObjectCommand_Ptr> AiBipedGotoPositionBehaviour::generate_commands()
{
	// NYI
	throw 23;
}

}
