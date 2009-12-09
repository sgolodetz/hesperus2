/***
 * hesperus: GameState.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "GameState.h"

#include <hesp/gui/Screen.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
GameState::GameState(const std::string& name)
:	FSMState(name)
{}

//#################### PROTECTED METHODS ####################
void GameState::set_display(const GUIComponent_Ptr& display)
{
	Screen& screen = Screen::instance();
	screen.layout().clear();
	screen.layout().add(display, screen.dimensions());
	screen.fit();
}

}
