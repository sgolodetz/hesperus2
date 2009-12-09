/***
 * ScarletPimpernel: Game.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Game.h"

#include <iostream>

#include <GL/glew.h>

#include <ASXEngine.h>

#include <hesp/gui/Screen.h>
#include <hesp/io/util/DirectoryFinder.h>
#include <hesp/util/ConfigOptions.h>
namespace bf = boost::filesystem;

namespace hesp {

//#################### CONSTRUCTORS ####################
Game::Game(const GameFSM_Ptr& fsm, const std::string& initialState, const GameData_Ptr& data)
try
:	m_data(data), m_fsm(fsm), m_mouseMotionSinceStateChange(false)
{
	// Read in the configuration options.
	ASXEngine configEngine;
	bf::path scriptsDir = DirectoryFinder::instance().determine_scripts_directory();
	if(!configEngine.load_and_build_script((scriptsDir / "config.as").file_string(), "config"))
	{
		configEngine.output_messages(std::cout);
		quit_with_error("Could not load configuration options - check the config.as script for problems");
	}
	ASXModule_Ptr configModule = configEngine.get_module("config");

	// FIXME: These should eventually be loaded in in a more sensible way.
	ConfigOptions& options = ConfigOptions::instance();
	options.set("width",		configModule->get_global_variable<int>("width"));
	options.set("height",		configModule->get_global_variable<int>("height"));
	options.set("fullScreen",	configModule->get_global_variable<bool>("fullScreen"));
	options.set("levelName",	configModule->get_global_variable<std::string>("levelName"));
	options.set("profile",		configModule->get_global_variable<std::string>("profile"));

	int width						= options.get<int>("width");
	int height						= options.get<int>("height");
	bool fullScreen					= options.get<bool>("fullScreen");
	const std::string& levelName	= options.get<std::string>("levelName");

	// Set up the window.
	if(SDL_Init(SDL_INIT_VIDEO) < 0) quit(EXIT_FAILURE);

	SDL_WM_SetCaption("The Scarlet Pimpernel", NULL);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	int bpp = 32;
	int flags = SDL_OPENGL;
	if(fullScreen) flags |= SDL_FULLSCREEN;

	if(SDL_SetVideoMode(width, height, bpp, flags) == 0) quit(EXIT_FAILURE);

	Screen& screen = Screen::instance();
	screen.set_dimensions(Extents(0, 0, width, height));
	screen.fit();

	if(glewInit() != GLEW_OK) quit_with_error("GLEW could not be initialised");
	if(!glewGetExtension("GL_ARB_multitexture")) quit_with_error("Multitexturing not supported");

	// Set the initial game state.
	m_fsm->set_initial_state(initialState);
}
catch(Exception& e) { quit_with_error(e.cause()); }

//#################### PUBLIC METHODS ####################
void Game::run()
{
	const Screen& screen = Screen::instance();

	Uint32 lastDraw = SDL_GetTicks();
	for(;;)
	{
		if(m_data->m_quitRequested) quit(0);

		process_events();

		Uint32 frameTime = SDL_GetTicks();
		Uint32 timeElapsed = frameTime - lastDraw;
		if(timeElapsed >= 16)	// aim for 62.5 frames per second
		{
#if 0
			std::cout << "Render " << timeElapsed << std::endl;
#endif

			screen.render();
			lastDraw = frameTime;

			// Note:	We clamp the elapsed time to 50ms to prevent things moving
			//			too far between one frame and the next.
			m_data->m_milliseconds = std::min(timeElapsed, Uint32(50));
			bool changedState = m_fsm->execute();
			m_data->m_input.set_mouse_motion(0, 0);

			if(changedState)
			{
				lastDraw = SDL_GetTicks();
				m_mouseMotionSinceStateChange = false;
			}
		}
	}
}

//#################### PRIVATE METHODS ####################
void Game::handle_key_down(const SDL_keysym& keysym)
{
	m_data->m_input.press_key(keysym.sym);
}

void Game::handle_key_up(const SDL_keysym& keysym)
{
	m_data->m_input.release_key(keysym.sym);
}

void Game::handle_mousebutton_down(const SDL_MouseButtonEvent& e)
{
	switch(e.button)
	{
		case SDL_BUTTON_LEFT:
			m_data->m_input.press_mouse_button(MOUSE_BUTTON_LEFT, e.x, e.y);
			break;
		case SDL_BUTTON_MIDDLE:
			m_data->m_input.press_mouse_button(MOUSE_BUTTON_MIDDLE, e.x, e.y);
			break;
		case SDL_BUTTON_RIGHT:
			m_data->m_input.press_mouse_button(MOUSE_BUTTON_RIGHT, e.x, e.y);
			break;
		default:
			break;
	}
}

void Game::handle_mousebutton_up(const SDL_MouseButtonEvent& e)
{
	switch(e.button)
	{
		case SDL_BUTTON_LEFT:
			m_data->m_input.release_mouse_button(MOUSE_BUTTON_LEFT);
			break;
		case SDL_BUTTON_MIDDLE:
			m_data->m_input.release_mouse_button(MOUSE_BUTTON_MIDDLE);
			break;
		case SDL_BUTTON_RIGHT:
			m_data->m_input.release_mouse_button(MOUSE_BUTTON_RIGHT);
			break;
		default:
			break;
	}
}

void Game::process_events()
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_KEYDOWN:
				handle_key_down(event.key.keysym);
				break;
			case SDL_KEYUP:
				handle_key_up(event.key.keysym);
				break;
			case SDL_MOUSEBUTTONDOWN:
				handle_mousebutton_down(event.button);
				break;
			case SDL_MOUSEBUTTONUP:
				handle_mousebutton_up(event.button);
				break;
			case SDL_MOUSEMOTION:
				m_data->m_input.set_mouse_position(event.motion.x, event.motion.y);
				if(m_mouseMotionSinceStateChange) m_data->m_input.set_mouse_motion(event.motion.xrel, event.motion.yrel);
				else m_mouseMotionSinceStateChange = true;
				break;
			case SDL_QUIT:
				quit(0);
				break;
			default:
				break;
		}
	}
}

void Game::quit(int code)
{
	SDL_Quit();
	exit(code);
}

void Game::quit_with_error(const std::string& error)
{
	SDL_Quit();
	std::cout << "Error: " << error << std::endl;
	exit(EXIT_FAILURE);
}

}
