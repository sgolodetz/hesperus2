/***
 * hesperus: MouseButton.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MOUSEBUTTON
#define H_HESP_MOUSEBUTTON

#include <sdl_mouse.h>

namespace hesp {

enum MouseButton
{
	MOUSE_BUTTON_LEFT = SDL_BUTTON_LEFT,
	MOUSE_BUTTON_MIDDLE = SDL_BUTTON_MIDDLE,
	MOUSE_BUTTON_RIGHT = SDL_BUTTON_RIGHT,
	MOUSE_BUTTON_LAST
};

}

#endif
