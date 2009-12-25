/***
 * hesperus: AiBipedRunTowardsBehaviour.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "AiBipedRunTowardsBehaviour.h"

#include <hesp/objects/commands/CmdBipedRun.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
AiBipedRunTowardsBehaviour::AiBipedRunTowardsBehaviour(const ObjectID& objectID, const ObjectManager *objectManager, const Vector3d& dest)
:	AiBipedMoveTowardsBehaviour(objectID, objectManager, dest)
{}

//#################### PRIVATE METHODS ####################
ObjectCommand_Ptr AiBipedRunTowardsBehaviour::generate_command(const ObjectID& objectID, const Vector3d& dir) const
{
	return ObjectCommand_Ptr(new CmdBipedRun(objectID, dir));
}

double AiBipedRunTowardsBehaviour::success_radius() const
{
	// FIXME: This should depend on the biped's speed.
	return 0.1;
}

}
