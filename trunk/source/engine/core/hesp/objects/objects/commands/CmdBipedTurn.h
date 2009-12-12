/***
 * hesperus: CmdBipedTurn.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMDBIPEDTURN
#define H_HESP_CMDBIPEDTURN

#include <hesp/level/objects/base/ObjectCommand.h>

namespace hesp {

class CmdBipedTurn : public ObjectCommand
{
	//#################### PRIVATE VARIABLES ####################
private:
	ObjectID m_objectID;
	int m_mouseMotionX, m_mouseMotionY;

	//#################### CONSTRUCTORS ####################
public:
	CmdBipedTurn(const ObjectID& objectID, int mouseMotionX, int mouseMotionY);

	//#################### PUBLIC METHODS ####################
public:
	void execute(const ObjectManager_Ptr& objectManager, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_CPtr& tree, const NavManager_CPtr& navManager, int milliseconds);
};

}

#endif
