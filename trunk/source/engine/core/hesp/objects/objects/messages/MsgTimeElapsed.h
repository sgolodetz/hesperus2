/***
 * hesperus: MsgTimeElapsed.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MSGTIMEELAPSED
#define H_HESP_MSGTIMEELAPSED

#include <hesp/level/objects/base/Message.h>

namespace hesp {

class MsgTimeElapsed : public Message
{
	//#################### PRIVATE VARIABLES ####################
private:
	int m_milliseconds;

	//#################### CONSTRUCTORS ####################
public:
	explicit MsgTimeElapsed(int milliseconds);

	//#################### PUBLIC METHODS ####################
public:
	void dispatch(MessageHandlerBase *handler) const;
	int milliseconds() const;
	std::set<ObjectID> referenced_objects() const;
};

}

#endif
