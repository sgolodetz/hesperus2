/***
 * ScarletPimpernel: GameTransition_LevelLoaded.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "GameTransition_LevelLoaded.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
GameTransition_LevelLoaded::GameTransition_LevelLoaded(const GameData_Ptr& gameData)
:	GameFSMTransition("LevelLoaded", "LoadLevel", "Level"), m_gameData(gameData)
{}

//#################### PUBLIC METHODS ####################
std::string GameTransition_LevelLoaded::execute()
{
	return to();
}

bool GameTransition_LevelLoaded::triggered() const
{
	return m_gameData->m_level != NULL;
}

}
