/***
 * hesperus: CmdBipedSetLook.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMDBIPEDSETLOOK
#define H_HESP_CMDBIPEDSETLOOK

#include <hesp/objects/base/ObjectCommand.h>

namespace hesp {

class CmdBipedSetLook : public ObjectCommand
{
	//#################### PRIVATE VARIABLES ####################
private:
	ObjectID m_objectID;
	Vector3d m_look;

	//#################### CONSTRUCTORS ####################
public:
	CmdBipedSetLook(const ObjectID& objectID, const Vector3d& look);

	//#################### PUBLIC METHODS ####################
public:
	void execute(const ObjectManager_Ptr& objectManager, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_CPtr& tree, const NavManager_CPtr& navManager, int milliseconds);
};

}

#endif
