/***
 * hesperus: InputState.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "InputState.h"

#include <hesp/exceptions/Exception.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
InputState::InputState()
{
	reset();
}

//#################### PUBLIC METHODS ####################
bool InputState::key_down(SDLKey key) const
{
	return m_keyDown[key];
}

bool InputState::mouse_button_down(MouseButton button) const
{
	return m_mouseButtonDown[button];
}

int InputState::mouse_motion_x() const
{
	return m_mouseMotionX;
}

int InputState::mouse_motion_y() const
{
	return m_mouseMotionY;
}

bool InputState::mouse_position_known() const
{
	return m_mousePositionX != -1;
}

int InputState::mouse_position_x() const
{
	if(m_mousePositionX != -1) return m_mousePositionX;
	else throw Exception("The mouse position is not yet known");
}

int InputState::mouse_position_y() const
{
	if(m_mousePositionY != -1) return m_mousePositionY;
	else throw Exception("The mouse position is not yet known");
}

int InputState::mouse_pressed_x(MouseButton button) const
{
	if(m_mousePressedX[button] != -1) return m_mousePressedX[button];
	else throw Exception("The specified mouse button is not currently pressed");
}

int InputState::mouse_pressed_y(MouseButton button) const
{
	if(m_mousePressedY[button] != -1) return m_mousePressedY[button];
	else throw Exception("The specified mouse button is not currently pressed");
}

void InputState::press_key(SDLKey key)
{
	m_keyDown[key] = true;
}

void InputState::press_mouse_button(MouseButton button, int x, int y)
{
	m_mouseButtonDown[button] = true;
	m_mousePressedX[button] = x;
	m_mousePressedY[button] = y;
}

void InputState::release_key(SDLKey key)
{
	m_keyDown[key] = false;
}

void InputState::release_mouse_button(MouseButton button)
{
	m_mouseButtonDown[button] = false;
	m_mousePressedX[button] = m_mousePressedY[button] = -1;
}

void InputState::reset()
{
	m_keyDown = boost::dynamic_bitset<>(SDLK_LAST, false);
	m_mouseButtonDown = boost::dynamic_bitset<>(MOUSE_BUTTON_LAST, false);
	set_mouse_motion(0, 0);
	set_mouse_position(-1, -1);
	m_mousePressedX = m_mousePressedY = std::vector<int>(MOUSE_BUTTON_LAST, -1);
}

void InputState::set_mouse_motion(int x, int y)
{
	m_mouseMotionX = x;
	m_mouseMotionY = y;
}

void InputState::set_mouse_position(int x, int y)
{
	m_mousePositionX = x;
	m_mousePositionY = y;
}

}
