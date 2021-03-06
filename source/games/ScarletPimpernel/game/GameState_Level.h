/***
 * ScarletPimpernel: GameState_Level.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_SCARLETPIMPERNEL_GAMESTATE_LEVEL
#define H_SCARLETPIMPERNEL_GAMESTATE_LEVEL

#include "GameState.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class GameData> GameData_Ptr;

class GameState_Level : public GameState
{
	//#################### PRIVATE VARIABLES ####################
private:
	GameData_Ptr m_gameData;
	bool m_inputGrabbed;

	// Flags
	bool m_pauseLevelFlag;

	//#################### CONSTRUCTORS ####################
public:
	GameState_Level(const GameData_Ptr& gameData);

	//#################### PUBLIC METHODS ####################
public:
	void enter();
	void execute();
	void leave();
	bool pause_level_flag() const;

	//#################### PRIVATE METHODS ####################
private:
	GUIComponent_Ptr construct_display();
	void grab_input();
	void ungrab_input();
};

}

#endif
