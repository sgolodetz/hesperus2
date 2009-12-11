/***
 * ScarletPimpernel: GameState_MainMenu.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_SCARLETPIMPERNEL_GAMESTATE_MAINMENU
#define H_SCARLETPIMPERNEL_GAMESTATE_MAINMENU

#include "GameState_ButtonsMenu.h"

namespace hesp {

class GameState_MainMenu : public GameState_ButtonsMenu
{
	//#################### PRIVATE VARIABLES ####################
private:
	bool m_newGamePressed;

	//#################### CONSTRUCTORS ####################
public:
	GameState_MainMenu(const GameData_Ptr& gameData);

	//#################### PUBLIC METHODS ####################
public:
	void enter();
	void leave();

	bool new_game_pressed() const;

	//#################### PRIVATE METHODS ####################
private:
	std::vector<Button*> menu_buttons();

	//#################### BUTTON HANDLERS ####################
private:
	void on_released_ExitGame();
	void on_released_NewGame();
};

}

#endif
