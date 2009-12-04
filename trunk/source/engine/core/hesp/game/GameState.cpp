/***
 * hesperus: GameState.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "GameState.h"

#include <hesp/gui/Screen.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
GameState::GameState()
:	m_quitFlag(false)
{}

//#################### DESTRUCTOR ####################
GameState::~GameState() {}

//#################### PUBLIC METHODS ####################
bool GameState::quit_requested() const
{
	return m_quitFlag;
}

//#################### PROTECTED METHODS ####################
void GameState::set_display(const GUIComponent_Ptr& display)
{
	Screen& screen = Screen::instance();
	screen.layout().clear();
	screen.layout().add(display, screen.dimensions());
	screen.fit();
}

void GameState::set_quit_flag()
{
	m_quitFlag = true;
}

}
