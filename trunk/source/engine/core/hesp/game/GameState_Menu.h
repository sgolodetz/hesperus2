/***
 * hesperus: GameState_Menu.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_GAMESTATE_MENU
#define H_HESP_GAMESTATE_MENU

#include <vector>

#include "GameState.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
class Button;
class SoundSystem;

class GameState_Menu : public GameState
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::string m_menu;
	GameState_Ptr m_newState;
	SoundSystem& m_soundSystem;

	// TODO: This should ultimately be removed in favour of loading the game levels in order.
	std::string m_initialLevelFilename;

	//#################### CONSTRUCTORS ####################
public:
	explicit GameState_Menu(const std::string& menu, const std::string& initialLevelFilename, SoundSystem& soundSystem);

	//#################### PUBLIC METHODS ####################
public:
	void enter();
	void leave();
	GameState_Ptr update(int milliseconds, InputState& input);

	//#################### PRIVATE METHODS ####################
private:
	GUIComponent_Ptr construct_buttons_menu(const std::vector<Button*> menuButtons);
	std::vector<Button*> menu_buttons_main();

	//#################### BUTTON HANDLERS ####################
private:
	void on_released_ExitGame();
	void on_released_NewGame();
};

}

#endif
