/***
 * ScarletPimpernel: GameFSM.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "GameFSM.h"

#include <hesp/gui/Screen.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
GameFSMState::GameFSMState(const std::string& name)
:	FSMState<GameData>(name)
{}

//#################### PROTECTED METHODS ####################
void GameFSMState::set_display(const GUIComponent_Ptr& display)
{
	Screen& screen = Screen::instance();
	screen.layout().clear();
	screen.layout().add(display, screen.dimensions());
	screen.fit();
}

}
