/***
 * ScarletPimpernel: GameState_LoadLevel.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_SCARLETPIMPERNEL_GAMESTATE_LOADLEVEL
#define H_SCARLETPIMPERNEL_GAMESTATE_LOADLEVEL

#include "GameState.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<struct GameData> GameData_Ptr;

class GameState_LoadLevel : public GameState
{
	//#################### PRIVATE VARIABLES ####################
private:
	GameData_Ptr m_gameData;
	bool m_firstTime;

	//#################### CONSTRUCTORS ####################
public:
	GameState_LoadLevel(const GameData_Ptr& gameData);

	//#################### PUBLIC METHODS ####################
public:
	void enter();
	void execute();

	//#################### PRIVATE METHODS ####################
private:
	GUIComponent_Ptr construct_display();
};

}

#endif
