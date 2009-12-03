/***
 * hesperus: MsgObjectPredestroyed.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MSGOBJECTPREDESTROYED
#define H_HESP_MSGOBJECTPREDESTROYED

#include <hesp/level/objects/base/Message.h>
#include <hesp/level/objects/base/ObjectID.h>

namespace hesp {

/**
An 'object predestroyed' message is sent prior to object destruction to allow
owned/contained objects to be destroyed first.
*/
class MsgObjectPredestroyed : public Message
{
	//#################### PRIVATE VARIABLES ####################
private:
	ObjectID m_id;

	//#################### CONSTRUCTORS ####################
public:
	explicit MsgObjectPredestroyed(const ObjectID& id);

	//#################### PUBLIC METHODS ####################
public:
	void dispatch(MessageHandlerBase *handler) const;
	const ObjectID& object_id() const;
	std::set<ObjectID> referenced_objects() const;
};

}

#endif
