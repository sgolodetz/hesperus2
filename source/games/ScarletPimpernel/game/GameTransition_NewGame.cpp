/***
 * ScarletPimpernel: GameTransition_NewGame.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "GameTransition_NewGame.h"

#include <hesp/io/util/DirectoryFinder.h>
#include <hesp/util/ConfigOptions.h>
#include "GameData.h"
#include "GameState_MainMenu.h"

namespace bf = boost::filesystem;

namespace hesp {

//#################### CONSTRUCTORS ####################
GameTransition_NewGame::GameTransition_NewGame(const GameData_Ptr& gameData, const GameState_MainMenu_Ptr& mainMenuState)
:	FSMTransition("NewGame", "MainMenu", "LoadLevel"), m_gameData(gameData), m_mainMenuState(mainMenuState)
{}

//#################### PUBLIC METHODS ####################
void GameTransition_NewGame::execute()
{
	std::string levelName = ConfigOptions::instance().get<std::string>("levelName");
	bf::path levelsDir = DirectoryFinder::instance().determine_levels_directory();
	std::string relativeLevelPath = levelName + "/" + levelName + ".bsp";
	m_gameData->set_level_filename((levelsDir / relativeLevelPath).file_string());
}

bool GameTransition_NewGame::triggered() const
{
	return m_mainMenuState->new_game_flag();
}

}
