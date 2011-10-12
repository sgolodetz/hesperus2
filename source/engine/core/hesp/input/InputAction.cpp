/***
 * hesperus: InputAction.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "InputAction.h"

#include <iostream>
#include <string>

#include <hesp/exceptions/Exception.h>

namespace hesp {

//#################### GLOBAL OPERATORS ####################
std::ostream& operator<<(std::ostream& os, InputAction rhs)
{
	switch(rhs)
	{
		case ACT_CROUCH:		os << "Crouch";			break;
		case ACT_JUMP:			os << "Jump";			break;
		case ACT_MOVE_BACKWARD:	os << "MoveBackward";	break;
		case ACT_MOVE_FORWARD:	os << "MoveForward";	break;
		case ACT_STRAFE_LEFT:	os << "StrafeLeft";		break;
		case ACT_STRAFE_RIGHT:	os << "StrafeRight";	break;
		case ACT_USE_ITEM:		os << "UseItem";		break;
		case ACT_WALK:			os << "Walk";			break;
		default:				throw Exception("Forgot to handle one of the input action cases");
	}
	return os;
}

std::istream& operator>>(std::istream& is, InputAction& rhs)
{
	std::string s;
	is >> s;
	if(s == "Crouch")				rhs = ACT_CROUCH;
	else if(s == "Jump")			rhs = ACT_JUMP;
	else if(s == "MoveBackward")	rhs = ACT_MOVE_BACKWARD;
	else if(s == "MoveForward")		rhs = ACT_MOVE_FORWARD;
	else if(s == "StrafeLeft")		rhs = ACT_STRAFE_LEFT;
	else if(s == "StrafeRight")		rhs = ACT_STRAFE_RIGHT;
	else if(s == "UseItem")			rhs = ACT_USE_ITEM;
	else if(s == "Walk")			rhs = ACT_WALK;
	else throw Exception("Unexpected input action " + s);
	return is;
}

}
