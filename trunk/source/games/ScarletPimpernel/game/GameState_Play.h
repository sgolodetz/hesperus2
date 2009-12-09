/***
 * ScarletPimpernel: GameState_Play.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_SCARLETPIMPERNEL_GAMESTATE_PLAY
#define H_SCARLETPIMPERNEL_GAMESTATE_PLAY

#include "GameFSM.h"

namespace hesp {

class GameState_Play : public GameFSMState
{
	//#################### PRIVATE VARIABLES ####################
private:
	GameData_Ptr m_gameData;
	bool m_inputGrabbed;

	//#################### CONSTRUCTORS ####################
public:
	GameState_Play(const GameData_Ptr& gameData);

	//#################### PUBLIC METHODS ####################
public:
	void enter();
	void execute();
	void leave();

	//#################### PRIVATE METHODS ####################
private:
	GUIComponent_Ptr construct_display();
	void grab_input();
	void ungrab_input();
};

}

#endif
