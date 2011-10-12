/***
 * ScarletPimpernel: GameState_MainMenu.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "GameState_MainMenu.h"

#include <boost/bind.hpp>

#include <hesp/gui/Button.h>
#include <hesp/io/util/DirectoryFinder.h>
#include "GameData.h"

namespace bf = boost::filesystem;

namespace hesp {

//#################### CONSTRUCTORS ####################
GameState_MainMenu::GameState_MainMenu(const GameData_Ptr& gameData)
:	GameState_ButtonsMenu("MainMenu", gameData)
{}

//#################### PUBLIC METHODS ####################
void GameState_MainMenu::enter()
{
	GameState_ButtonsMenu::enter();

	// Reset the flags.
	m_newGameFlag = false;
}

bool GameState_MainMenu::new_game_flag() const
{
	return m_newGameFlag;
}

//#################### PRIVATE METHODS ####################
std::vector<Button*> GameState_MainMenu::menu_buttons()
{
	std::vector<Button*> ret;
	bf::path imagesDir = DirectoryFinder::instance().determine_images_directory();

	ret.push_back(new Button((imagesDir / "buttons-newgame-inactive.png").file_string(),
							 (imagesDir / "buttons-newgame-active.png").file_string(),
							 boost::none, boost::bind(&GameState_MainMenu::on_released_NewGame, this)));

	ret.push_back(new Button((imagesDir / "buttons-exitgame-inactive.png").file_string(),
							 (imagesDir / "buttons-exitgame-active.png").file_string(),
							 boost::none, boost::bind(&GameState_MainMenu::on_released_ExitGame, this)));

	return ret;
}

//#################### BUTTON HANDLERS ####################
void GameState_MainMenu::on_released_ExitGame()
{
	m_gameData->set_quit_requested();
}

void GameState_MainMenu::on_released_NewGame()
{
	m_newGameFlag = true;
}

}
