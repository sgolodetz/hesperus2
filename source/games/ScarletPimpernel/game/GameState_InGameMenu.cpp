/***
 * ScarletPimpernel: GameState_InGameMenu.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "GameState_InGameMenu.h"

#include <boost/bind.hpp>

#include <hesp/gui/Button.h>
#include <hesp/io/util/DirectoryFinder.h>

namespace bf = boost::filesystem;

namespace hesp {

//#################### CONSTRUCTORS ####################
GameState_InGameMenu::GameState_InGameMenu(const GameData_Ptr& gameData)
:	GameState_ButtonsMenu("InGameMenu", gameData)
{}

//#################### PUBLIC METHODS ####################
void GameState_InGameMenu::enter()
{
	GameState_ButtonsMenu::enter();

	// Reset the flags.
	m_exitLevelFlag = false;
	m_resumeLevelFlag = false;
}

bool GameState_InGameMenu::exit_level_flag() const
{
	return m_exitLevelFlag;
}

bool GameState_InGameMenu::resume_level_flag() const
{
	return m_resumeLevelFlag;
}

//#################### PRIVATE METHODS ####################
std::vector<Button*> GameState_InGameMenu::menu_buttons()
{
	std::vector<Button*> ret;
	bf::path imagesDir = DirectoryFinder::instance().determine_images_directory();

	ret.push_back(new Button((imagesDir / "buttons-resumelevel-inactive.png").file_string(),
							 (imagesDir / "buttons-resumelevel-active.png").file_string(),
							 boost::none, boost::bind(&GameState_InGameMenu::on_released_ResumeLevel, this)));

	ret.push_back(new Button((imagesDir / "buttons-exitlevel-inactive.png").file_string(),
							 (imagesDir / "buttons-exitlevel-active.png").file_string(),
							 boost::none, boost::bind(&GameState_InGameMenu::on_released_ExitLevel, this)));

	return ret;
}

//#################### BUTTON HANDLERS ####################
void GameState_InGameMenu::on_released_ExitLevel()
{
	m_exitLevelFlag = true;
}

void GameState_InGameMenu::on_released_ResumeLevel()
{
	m_resumeLevelFlag = true;
}

}
