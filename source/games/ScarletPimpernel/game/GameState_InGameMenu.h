/***
 * ScarletPimpernel: GameState_InGameMenu.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_SCARLETPIMPERNEL_GAMESTATE_INGAMEMENU
#define H_SCARLETPIMPERNEL_GAMESTATE_INGAMEMENU

#include "GameState_ButtonsMenu.h"

namespace hesp {

class GameState_InGameMenu : public GameState_ButtonsMenu
{
	//#################### PRIVATE VARIABLES ####################
private:
	bool m_exitLevelFlag;
	bool m_resumeLevelFlag;

	//#################### CONSTRUCTORS ####################
public:
	explicit GameState_InGameMenu(const GameData_Ptr& gameData);

	//#################### PUBLIC METHODS ####################
public:
	void enter();
	bool exit_level_flag() const;
	bool resume_level_flag() const;

	//#################### PRIVATE METHODS ####################
private:
	std::vector<Button*> menu_buttons();

	//#################### BUTTON HANDLERS ####################
private:
	void on_released_ExitLevel();
	void on_released_ResumeLevel();
};

}

#endif
