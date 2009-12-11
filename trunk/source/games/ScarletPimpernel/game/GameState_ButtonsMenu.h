/***
 * ScarletPimpernel: GameState_ButtonsMenu.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_SCARLETPIMPERNEL_GAMESTATE_BUTTONSMENU
#define H_SCARLETPIMPERNEL_GAMESTATE_BUTTONSMENU

#include <vector>

#include "GameState.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
class Button;
typedef shared_ptr<struct GameData> GameData_Ptr;

class GameState_ButtonsMenu : public GameState
{
	//#################### PROTECTED VARIABLES ####################
protected:
	GameData_Ptr m_gameData;

	//#################### CONSTRUCTORS ####################
public:
	explicit GameState_ButtonsMenu(const std::string& name, const GameData_Ptr& gameData);

	//#################### PUBLIC METHODS ####################
public:
	void execute();

	//#################### PROTECTED METHODS ####################
protected:
	GUIComponent_Ptr construct_menu(const std::vector<Button*> buttons);
};

}

#endif
