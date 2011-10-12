/***
 * ScarletPimpernel: GameTransition_PauseLevel.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "GameTransition_PauseLevel.h"

#include "GameState_Level.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
GameTransition_PauseLevel::GameTransition_PauseLevel(const GameState_Level_Ptr& levelState)
:	FSMTransition("PauseLevel", "Level", "InGameMenu"), m_levelState(levelState)
{}

//#################### PUBLIC METHODS ####################
bool GameTransition_PauseLevel::triggered() const
{
	return m_levelState->pause_level_flag();
}

}
