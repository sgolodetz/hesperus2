/***
 * hesperus: CmdBipedChangePosture.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMDBIPEDCHANGEPOSTURE
#define H_HESP_CMDBIPEDCHANGEPOSTURE

#include <hesp/objects/base/ObjectCommand.h>

namespace hesp {

class CmdBipedChangePosture : public ObjectCommand
{
	//#################### PRIVATE VARIABLES ####################
private:
	ObjectID m_objectID;

	//#################### CONSTRUCTORS ####################
public:
	CmdBipedChangePosture(const ObjectID& objectID);

	//#################### PUBLIC METHODS ####################
public:
	void execute(const ObjectManager_Ptr& objectManager, int milliseconds);
};

}

#endif
