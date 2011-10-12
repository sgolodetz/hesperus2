/***
 * hesperus: MsgObjectPredestroyed.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "MsgObjectPredestroyed.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
MsgObjectPredestroyed::MsgObjectPredestroyed(const ObjectID& id)
:	m_id(id)
{}

//#################### PUBLIC METHODS ####################
void MsgObjectPredestroyed::dispatch(MessageHandlerBase *handler) const
{
	dynamic_dispatch(handler, *this);
}

const ObjectID& MsgObjectPredestroyed::object_id() const
{
	return m_id;
}

std::set<ObjectID> MsgObjectPredestroyed::referenced_objects() const
{
	std::set<ObjectID> ret;
	ret.insert(m_id);
	return ret;
}

}
