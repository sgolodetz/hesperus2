/***
 * hesperus: InputState.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_INPUTSTATE
#define H_HESP_INPUTSTATE

#include <boost/dynamic_bitset.hpp>

#include <SDL_keysym.h>

#include "MouseButton.h"

namespace hesp {

class InputState
{
	//#################### PRIVATE VARIABLES ####################
private:
	boost::dynamic_bitset<> m_keyDown;
	boost::dynamic_bitset<> m_mouseButtonDown;
	int m_mouseMotionX, m_mouseMotionY;
	int m_mousePositionX, m_mousePositionY;
	std::vector<int> m_mousePressedX, m_mousePressedY;

	//#################### CONSTRUCTORS ####################
public:
	InputState();

	//#################### COPY CONSTRUCTOR & ASSIGNMENT OPERATOR ####################
private:
	InputState(const InputState&);
	InputState& operator=(const InputState&);

	//#################### PUBLIC METHODS ####################
public:
	bool key_down(SDLKey key) const;
	bool mouse_button_down(MouseButton button) const;
	int mouse_motion_x() const;
	int mouse_motion_y() const;
	bool mouse_position_known() const;
	int mouse_position_x() const;
	int mouse_position_y() const;
	int mouse_pressed_x(MouseButton button) const;
	int mouse_pressed_y(MouseButton button) const;
	void press_key(SDLKey key);
	void press_mouse_button(MouseButton button, int x, int y);
	void release_key(SDLKey key);
	void release_mouse_button(MouseButton button);
	void reset();
	void set_mouse_motion(int x, int y);
	void set_mouse_position(int x, int y);
};

}

#endif
