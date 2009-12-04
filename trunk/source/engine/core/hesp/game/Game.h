/***
 * hesperus: Game.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_GAME
#define H_HESP_GAME

#include <string>

#include <SDL.h>

#include <hesp/audio/SoundSystem.h>
#include <hesp/input/InputState.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class GameState> GameState_Ptr;

class Game
{
	//#################### PRIVATE VARIABLES ####################
private:
	GameState_Ptr m_state;
	InputState m_input;
	SoundSystem m_soundSystem;

	bool m_mouseMotionSinceStateChange;		// whether or not the mouse has been moved since the last state change

	//#################### CONSTRUCTORS ####################
public:
	Game();

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
