/***
 * ScarletPimpernel: GameRunner.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_SCARLETPIMPERNEL_GAMERUNNER
#define H_SCARLETPIMPERNEL_GAMERUNNER

#include "GameFSM.h"

#include <string>

#include <SDL.h>

namespace hesp {

class GameRunner
{
	//#################### PRIVATE VARIABLES ####################
private:
	GameData_Ptr m_data;
	GameFSM_Ptr m_fsm;
	bool m_mouseMotionSinceStateChange;		// whether or not the mouse has been moved since the last state change

	//#################### CONSTRUCTORS ####################
public:
	GameRunner(const GameFSM_Ptr& fsm, const std::string& initialState, const GameData_Ptr& data);

	//#################### PUBLIC METHODS ####################
public:
	void run();

	//#################### PRIVATE METHODS ####################
private:
	void handle_key_down(const SDL_keysym& keysym);
	void handle_key_up(const SDL_keysym& keysym);
	void handle_mousebutton_down(const SDL_MouseButtonEvent& e);
	void handle_mousebutton_up(const SDL_MouseButtonEvent& e);
	void process_events();
	void quit(int code);
	void quit_with_error(const std::string& error);
};

}

#endif
