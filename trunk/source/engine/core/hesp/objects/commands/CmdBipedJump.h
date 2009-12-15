/***
 * hesperus: CmdBipedJump.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMDBIPEDJUMP
#define H_HESP_CMDBIPEDJUMP

#include <hesp/math/vectors/Vector3.h>
#include <hesp/objects/base/ObjectCommand.h>

namespace hesp {

class CmdBipedJump : public ObjectCommand
{
	//#################### PRIVATE VARIABLES ####################
private:
	ObjectID m_objectID;
	Vector3d m_dir;

	//#################### CONSTRUCTORS ####################
public:
	CmdBipedJump(const ObjectID& objectID, const Vector3d& dir);

	//#################### PUBLIC METHODS ####################
public:
	void execute(const ObjectManager_Ptr& objectManager, int milliseconds);
};

}

#endif
