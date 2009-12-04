/***
 * hesperus: GameState_Load.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_GAMESTATE_LOAD
#define H_HESP_GAMESTATE_LOAD

#include <string>

#include "GameState.h"

namespace hesp {

class GameState_Load : public GameState
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::string m_levelFilename;
	bool m_firstTime;

	//#################### CONSTRUCTORS ####################
public:
	GameState_Load(const std::string& levelFilename);

	//#################### PUBLIC METHODS ####################
public:
	void enter();
	void leave();
	GameState_Ptr update(int milliseconds, InputState& input);

	//#################### PRIVATE METHODS ####################
private:
	GUIComponent_Ptr construct_display();
};

}

#endif
