/***
 * hesperus: CmdUseActiveItem.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMDUSEACTIVEITEM
#define H_HESP_CMDUSEACTIVEITEM

#include <hesp/level/objects/base/ObjectCommand.h>

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
	void execute(const ObjectManager_Ptr& objectManager, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_CPtr& tree, const NavManager_CPtr& navManager, int milliseconds);
};

}

#endif
