/***
 * hesperus: ObjectCommand.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_OBJECTCOMMAND
#define H_HESP_OBJECTCOMMAND

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <hesp/objects/base/ObjectID.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class ObjectManager> ObjectManager_Ptr;

class ObjectCommand
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~ObjectCommand() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void execute(const ObjectManager_Ptr& objectManager, int milliseconds) = 0;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ObjectCommand> ObjectCommand_Ptr;
typedef shared_ptr<const ObjectCommand> ObjectCommand_CPtr;

}

#endif
