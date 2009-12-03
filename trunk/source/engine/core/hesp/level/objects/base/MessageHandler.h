/***
 * hesperus: MessageHandler.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MESSAGEHANDLER
#define H_HESP_MESSAGEHANDLER

namespace hesp {

class MessageHandlerBase
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~MessageHandlerBase() {}
};

template <typename MessageType>
class MessageHandler : public virtual MessageHandlerBase
{
	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void process_message(const MessageType& msg) = 0;
};

}

#endif
