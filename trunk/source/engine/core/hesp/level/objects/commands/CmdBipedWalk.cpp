/***
 * hesperus: CmdBipedWalk.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmdBipedWalk.h"

#include <hesp/level/objects/components/ICmpBipedAnimChooser.h>
#include <hesp/level/objects/components/ICmpMovement.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
CmdBipedWalk::CmdBipedWalk(const ObjectID& objectID, const Vector3d& dir)
:	CmdBipedMove(objectID, dir)
{}

//#################### PRIVATE METHODS ####################
double CmdBipedWalk::lookup_speed(const ICmpMovement_CPtr& cmpMovement) const
{
	return cmpMovement->walk_speed();
}

void CmdBipedWalk::set_anim_flag(const ICmpBipedAnimChooser_Ptr& cmpAnimChooser) const
{
	cmpAnimChooser->set_walk_flag();
}

}
