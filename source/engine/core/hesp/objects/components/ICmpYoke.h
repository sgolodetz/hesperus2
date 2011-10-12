/***
 * hesperus: ICmpYoke.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICMPYOKE
#define H_HESP_ICMPYOKE

#include <hesp/objects/base/ObjectComponent.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
class InputState;
typedef shared_ptr<class ObjectCommand> ObjectCommand_Ptr;

class ICmpYoke : public ObjectComponent
{
	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual std::vector<ObjectCommand_Ptr> generate_commands(InputState& input) = 0;

	//#################### PUBLIC METHODS ####################
public:
	std::string group_type() const			{ return "Yoke"; }
	static std::string static_group_type()	{ return "Yoke"; }

	std::string own_type() const			{ return "Yoke"; }
	static std::string static_own_type()	{ return "Yoke"; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ICmpYoke> ICmpYoke_Ptr;
typedef shared_ptr<const ICmpYoke> ICmpYoke_CPtr;

}

#endif
