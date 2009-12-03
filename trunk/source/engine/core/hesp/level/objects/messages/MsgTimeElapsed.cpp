/***
 * hesperus: MsgTimeElapsed.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "MsgTimeElapsed.h"

#include <hesp/level/objects/base/ObjectID.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
MsgTimeElapsed::MsgTimeElapsed(int milliseconds)
:	m_milliseconds(milliseconds)
{}

//#################### PUBLIC METHODS ####################
void MsgTimeElapsed::dispatch(MessageHandlerBase *handler) const
{
	dynamic_dispatch(handler, *this);
}

int MsgTimeElapsed::milliseconds() const
{
	return m_milliseconds;
}

std::set<ObjectID> MsgTimeElapsed::referenced_objects() const
{
	return std::set<ObjectID>();
}

}
