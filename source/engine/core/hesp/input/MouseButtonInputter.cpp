/***
 * hesperus: MouseButtonInputter.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "MouseButtonInputter.h"

#include "InputState.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
MouseButtonInputter::MouseButtonInputter(MouseButton button)
:	m_button(button)
{}

//#################### PUBLIC METHODS ####################
bool MouseButtonInputter::down(const InputState& input) const
{
	return input.mouse_button_down(m_button);
}

void MouseButtonInputter::release(InputState& input) const
{
	input.release_mouse_button(m_button);
}

}
