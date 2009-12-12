/***
 * hesperus: UserBipedYoke.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_USERBIPEDYOKE
#define H_HESP_USERBIPEDYOKE

#include <hesp/level/objects/base/IYoke.h>
#include <hesp/level/objects/base/ObjectID.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<const class InputBinding> InputBinding_CPtr;
class ObjectManager;

/**
This class represents a yoke that allows the user to control a biped.
Bipeds can walk, run, jump, crouch, etc.
*/
class UserBipedYoke : public IYoke
{
	//#################### PRIVATE VARIABLES ####################
private:
	ObjectID m_objectID;
	ObjectManager *m_objectManager;
	InputBinding_CPtr m_inputBinding;

	//#################### CONSTRUCTORS ####################
public:
	UserBipedYoke(const ObjectID& objectID, ObjectManager *objectManager);

	//#################### PUBLIC METHODS ####################
public:
	std::vector<ObjectCommand_Ptr> generate_commands(InputState& input, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_CPtr& tree, const NavManager_CPtr& navManager);
};

}

#endif
