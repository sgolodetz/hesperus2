/***
 * hesperus: CmdBipedMove.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmdBipedMove.h"

#include <hesp/objects/components/ICmpBipedAnimChooser.h>
#include <hesp/objects/components/ICmpMovement.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
CmdBipedMove::CmdBipedMove(const ObjectID& objectID, const Vector3d& dir)
:	m_objectID(objectID), m_dir(dir)
{}

//#################### PUBLIC METHODS ####################
void CmdBipedMove::execute(const ObjectManager_Ptr& objectManager, int milliseconds)
{
	// Lookup the appropriate speed with which to move.
	ICmpMovement_Ptr cmpMovement = objectManager->get_component(m_objectID, cmpMovement);	assert(cmpMovement);
	double speed = lookup_speed(cmpMovement);

	// Actually move the object.
	cmpMovement->move(m_dir, speed, milliseconds);

	// Set the appropriate animation flag (e.g. walk, run, etc.)
	ICmpBipedAnimChooser_Ptr cmpAnimChooser = objectManager->get_component(m_objectID, cmpAnimChooser);
	if(cmpAnimChooser) set_anim_flag(cmpAnimChooser);
}

}
