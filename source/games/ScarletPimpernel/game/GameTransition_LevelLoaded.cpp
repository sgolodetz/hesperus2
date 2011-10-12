/***
 * ScarletPimpernel: GameTransition_LevelLoaded.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "GameTransition_LevelLoaded.h"

#include "GameData.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
GameTransition_LevelLoaded::GameTransition_LevelLoaded(const GameData_Ptr& gameData)
:	FSMTransition("LevelLoaded", "LoadLevel", "Level"), m_gameData(gameData)
{}

//#################### PUBLIC METHODS ####################
bool GameTransition_LevelLoaded::triggered() const
{
	return m_gameData->level() != NULL;
}

}
