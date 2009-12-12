/***
 * hesperus: ObjectCommand.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_OBJECTCOMMAND
#define H_HESP_OBJECTCOMMAND

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <hesp/level/objects/base/ObjectID.h>
#include <hesp/util/PolygonTypes.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<const class NavManager> NavManager_CPtr;
typedef shared_ptr<class ObjectManager> ObjectManager_Ptr;
typedef shared_ptr<const class OnionTree> OnionTree_CPtr;

class ObjectCommand
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~ObjectCommand() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void execute(const ObjectManager_Ptr& objectManager, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_CPtr& tree, const NavManager_CPtr& navManager, int milliseconds) = 0;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ObjectCommand> ObjectCommand_Ptr;
typedef shared_ptr<const ObjectCommand> ObjectCommand_CPtr;

}

#endif
