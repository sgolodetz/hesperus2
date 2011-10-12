/***
 * ScarletPimpernel: GameState_Level.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "GameState_Level.h"

#include <SDL.h>

#include <hesp/cameras/FirstPersonCamera.h>
#include <hesp/cameras/FixedCamera.h>
#include <hesp/gui/Picture.h>
#include <hesp/gui/Screen.h>
#include <hesp/io/util/DirectoryFinder.h>
#include <hesp/level/HUDViewer.h>
#include <hesp/level/LevelViewer.h>
#include <hesp/objects/base/ObjectManager.h>
#include "GameData.h"

namespace bf = boost::filesystem;

namespace hesp {

//#################### CONSTRUCTORS ####################
GameState_Level::GameState_Level(const GameData_Ptr& gameData)
:	GameState("Level"), m_gameData(gameData), m_inputGrabbed(false)
{}

//#################### PUBLIC METHODS ####################
void GameState_Level::enter()
{
	// Clear all pending SDL events before we get started.
	{ SDL_Event e; while(SDL_PollEvent(&e)) {} }

	set_display(construct_display());
	grab_input();

	// Reset the flags.
	m_pauseLevelFlag = false;
}

void GameState_Level::execute()
{
	if(m_gameData->input().key_down(SDLK_ESCAPE))
	{
		m_pauseLevelFlag = true;
		return;
	}

	// TEMPORARY: Allow quick toggling of the input grab using the 'g' key (for debugging purposes).
	if(m_gameData->input().key_down(SDLK_g))
	{
		m_gameData->input().release_key(SDLK_g);
		if(m_inputGrabbed) ungrab_input();
		else grab_input();
	}

	m_gameData->level()->update(m_gameData->milliseconds(), m_gameData->input());
}

void GameState_Level::leave()
{
	ungrab_input();
}

bool GameState_Level::pause_level_flag() const
{
	return m_pauseLevelFlag;
}

//#################### PRIVATE METHODS ####################
GUIComponent_Ptr GameState_Level::construct_display()
{
	GUIContainer<ExplicitLayout> *display = new GUIContainer<ExplicitLayout>;

	bf::path imagesDir = DirectoryFinder::instance().determine_images_directory();

	const Screen& screen = Screen::instance();
	int width = screen.dimensions().width();
	int height = screen.dimensions().height();
	display->layout().add(new Picture((imagesDir / "title.png").file_string()), Extents(width/4, 0, width*3/4, width/8));

	Extents mainExtents(50, width/8, width - 50, height - 50);

	Camera_Ptr camera(new FirstPersonCamera(m_gameData->level()->object_manager()->player(), m_gameData->level()->object_manager()));
	display->layout().add(new LevelViewer(m_gameData->level(), camera), mainExtents);

	display->layout().add(new HUDViewer(m_gameData->level()), mainExtents);

	return GUIComponent_Ptr(display);
}

void GameState_Level::grab_input()
{
	SDL_ShowCursor(SDL_DISABLE);
	SDL_WM_GrabInput(SDL_GRAB_ON);
	m_inputGrabbed = true;
}

void GameState_Level::ungrab_input()
{
	SDL_WM_GrabInput(SDL_GRAB_OFF);
	SDL_ShowCursor(SDL_ENABLE);
	m_inputGrabbed = false;
}

}
