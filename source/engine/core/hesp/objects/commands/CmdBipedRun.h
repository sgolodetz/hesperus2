/***
 * hesperus: CmdBipedRun.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMDBIPEDRUN
#define H_HESP_CMDBIPEDRUN

#include "CmdBipedMove.h"

namespace hesp {

class CmdBipedRun : public CmdBipedMove
{
	//#################### CONSTRUCTORS ####################
public:
	CmdBipedRun(const ObjectID& objectID, const Vector3d& dir);

	//#################### PRIVATE METHODS ####################
private:
	double lookup_speed(const ICmpMovement_CPtr& cmpMovement) const;
	void set_anim_flag(const ICmpBipedAnimChooser_Ptr& cmpAnimChooser) const;
};

}

#endif
