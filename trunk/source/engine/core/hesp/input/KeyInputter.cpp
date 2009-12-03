/***
 * hesperus: KeyInputter.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "KeyInputter.h"

#include "InputState.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
KeyInputter::KeyInputter(SDLKey key)
:	m_key(key)
{}

//#################### PUBLIC METHODS ####################
bool KeyInputter::down(const InputState& input) const
{
	return input.key_down(m_key);
}

void KeyInputter::release(InputState& input) const
{
	input.release_key(m_key);
}

}
