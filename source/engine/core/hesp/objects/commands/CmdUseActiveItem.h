/***
 * hesperus: CmdUseActiveItem.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMDUSEACTIVEITEM
#define H_HESP_CMDUSEACTIVEITEM

#include <hesp/objects/base/ObjectCommand.h>

namespace hesp {

class CmdUseActiveItem : public ObjectCommand
{
	//#################### PRIVATE VARIABLES ####################
private:
	ObjectID m_objectID;

	//#################### CONSTRUCTORS ####################
public:
	explicit CmdUseActiveItem(const ObjectID& objectID);

	//#################### PUBLIC METHODS ####################
public:
	void execute(const ObjectManager_Ptr& objectManager, int milliseconds);
};

}

#endif
