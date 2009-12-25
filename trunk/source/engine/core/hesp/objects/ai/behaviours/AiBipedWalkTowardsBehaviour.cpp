/***
 * hesperus: AiBipedWalkTowardsBehaviour.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "AiBipedWalkTowardsBehaviour.h"

#include <hesp/objects/commands/CmdBipedWalk.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
AiBipedWalkTowardsBehaviour::AiBipedWalkTowardsBehaviour(const ObjectID& objectID, const ObjectManager *objectManager, const Vector3d& dest)
:	AiBipedMoveTowardsBehaviour(objectID, objectManager, dest)
{}

//#################### PRIVATE METHODS ####################
ObjectCommand_Ptr AiBipedWalkTowardsBehaviour::generate_command(const ObjectID& objectID, const Vector3d& dir) const
{
	return ObjectCommand_Ptr(new CmdBipedWalk(objectID, dir));
}

double AiBipedWalkTowardsBehaviour::success_radius() const
{
	// FIXME: This should depend on the biped's speed.
	return 0.1;
}

}
