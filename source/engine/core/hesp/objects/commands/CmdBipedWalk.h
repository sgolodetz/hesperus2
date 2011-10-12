/***
 * hesperus: CmdBipedWalk.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMDBIPEDWALK
#define H_HESP_CMDBIPEDWALK

#include "CmdBipedMove.h"

namespace hesp {

class CmdBipedWalk : public CmdBipedMove
{
	//#################### CONSTRUCTORS ####################
public:
	CmdBipedWalk(const ObjectID& objectID, const Vector3d& dir);

	//#################### PRIVATE METHODS ####################
private:
	double lookup_speed(const ICmpMovement_CPtr& cmpMovement) const;
	void set_anim_flag(const ICmpBipedAnimChooser_Ptr& cmpAnimChooser) const;
};

}

#endif
