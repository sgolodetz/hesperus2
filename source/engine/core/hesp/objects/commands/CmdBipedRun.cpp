/***
 * hesperus: CmdBipedRun.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmdBipedRun.h"

#include <hesp/objects/components/ICmpBipedAnimChooser.h>
#include <hesp/objects/components/ICmpMovement.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
CmdBipedRun::CmdBipedRun(const ObjectID& objectID, const Vector3d& dir)
:	CmdBipedMove(objectID, dir)
{}

//#################### PRIVATE METHODS ####################
double CmdBipedRun::lookup_speed(const ICmpMovement_CPtr& cmpMovement) const
{
	return cmpMovement->run_speed();
}

void CmdBipedRun::set_anim_flag(const ICmpBipedAnimChooser_Ptr& cmpAnimChooser) const
{
	cmpAnimChooser->set_run_flag();
}

}
