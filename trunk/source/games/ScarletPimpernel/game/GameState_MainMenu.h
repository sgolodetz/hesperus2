/***
 * ScarletPimpernel: GameState_MainMenu.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_SCARLETPIMPERNEL_GAMESTATE_MAINMENU
#define H_SCARLETPIMPERNEL_GAMESTATE_MAINMENU

#include "GameFSM.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
class Button;

class GameState_MainMenu : public GameFSMState
{
	//#################### PRIVATE VARIABLES ####################
private:
	GameData_Ptr m_gameData;

	bool m_newGamePressed;

	//#################### CONSTRUCTORS ####################
public:
	GameState_MainMenu(const GameData_Ptr& gameData);

	//#################### PUBLIC METHODS ####################
public:
	void enter();
	void execute();
	void leave();

	bool new_game_pressed() const;

	//#################### PRIVATE METHODS ####################
private:
	GUIComponent_Ptr construct_buttons_menu(const std::vector<Button*> menuButtons);
	std::vector<Button*> menu_buttons();

	//#################### BUTTON HANDLERS ####################
private:
	void on_released_ExitGame();
	void on_released_NewGame();
};

}

#endif
