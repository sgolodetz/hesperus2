/***
 * hesperus: InputAction.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_INPUTACTION
#define H_HESP_INPUTACTION

#include <iosfwd>

namespace hesp {

//#################### ENUMERATIONS ####################
enum InputAction
{
	ACT_CROUCH,
	ACT_JUMP,
	ACT_MOVE_BACKWARD,
	ACT_MOVE_FORWARD,
	ACT_STRAFE_LEFT,
	ACT_STRAFE_RIGHT,
	ACT_USE_ITEM,
	ACT_WALK,
};

//#################### GLOBAL OPERATORS ####################
std::ostream& operator<<(std::ostream& os, InputAction rhs);
std::istream& operator>>(std::istream& is, InputAction& rhs);

}

#endif
