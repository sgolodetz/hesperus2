/***
 * hesperus: Message.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MESSAGE
#define H_HESP_MESSAGE

#include <set>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include "MessageHandler.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
class ObjectID;

// Note: The message-handling scheme as a whole is based on that described at www.ddj.com/cpp/184429055.
class Message
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~Message() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void dispatch(MessageHandlerBase *handler) const = 0;
	virtual std::set<ObjectID> referenced_objects() const = 0;

	//#################### PROTECTED METHODS ####################
protected:
	template <typename MessageType>
	static void dynamic_dispatch(MessageHandlerBase *handler, const MessageType& self)
	{
		MessageHandler<MessageType> *msgHandler = dynamic_cast<MessageHandler<MessageType>*>(handler);
		if(msgHandler) msgHandler->process_message(self);
	}
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Message> Message_Ptr;
typedef shared_ptr<const Message> Message_CPtr;

}

#endif
