/***
 * ScarletPimpernel: GameState_LoadLevel.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_SCARLETPIMPERNEL_GAMESTATE_LOADLEVEL
#define H_SCARLETPIMPERNEL_GAMESTATE_LOADLEVEL

#include "GameFSM.h"

namespace hesp {

class GameState_LoadLevel : public GameFSMState
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
