/***
 * ScarletPimpernel: GameTransition_ResumeLevel.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "GameTransition_ResumeLevel.h"

#include "GameState_InGameMenu.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
GameTransition_ResumeLevel::GameTransition_ResumeLevel(const GameState_InGameMenu_Ptr& inGameMenuState)
:	FSMTransition("ResumeLevel", "InGameMenu", "Level"), m_inGameMenuState(inGameMenuState)
{}

//#################### PUBLIC METHODS ####################
bool GameTransition_ResumeLevel::triggered() const
{
	return m_inGameMenuState->resume_level_flag();
}

}
