/***
 * ScarletPimpernel: GameTransition_ExitLevel.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "GameTransition_ExitLevel.h"

#include "GameData.h"
#include "GameState_InGameMenu.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
GameTransition_ExitLevel::GameTransition_ExitLevel(const GameData_Ptr& gameData, const GameState_InGameMenu_Ptr& inGameMenuState)
:	FSMTransition("ExitLevel", "InGameMenu", "MainMenu"), m_gameData(gameData), m_inGameMenuState(inGameMenuState)
{}

//#################### PUBLIC METHODS ####################
void GameTransition_ExitLevel::execute()
{
	// Not strictly necessary, but frees up memory.
	m_gameData->set_level(Level_Ptr());
	m_gameData->set_level_filename("");
}

bool GameTransition_ExitLevel::triggered() const
{
	return m_inGameMenuState->exit_level_flag();
}

}
