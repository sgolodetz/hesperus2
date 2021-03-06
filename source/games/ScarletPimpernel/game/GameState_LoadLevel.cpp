/***
 * ScarletPimpernel: GameState_LoadLevel.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "GameState_LoadLevel.h"

#include <hesp/exceptions/FileNotFoundException.h>
#include <hesp/gui/Picture.h>
#include <hesp/gui/Screen.h>
#include <hesp/io/files/LevelFile.h>
#include <hesp/io/util/DirectoryFinder.h>
#include "GameData.h"

namespace bf = boost::filesystem;

namespace hesp {

//#################### CONSTRUCTORS ####################
GameState_LoadLevel::GameState_LoadLevel(const GameData_Ptr& gameData)
:	GameState("LoadLevel"), m_gameData(gameData), m_firstTime(true)
{}

//#################### PUBLIC METHODS ####################
void GameState_LoadLevel::enter()
{
	set_display(construct_display());
	m_gameData->set_level(Level_Ptr());
}

void GameState_LoadLevel::execute()
{
	// Ensure that the loading screen's been rendered before we try and load the level (render happens after updating).
	if(m_firstTime) m_firstTime = false;
	else m_gameData->set_level(LevelFile::load(m_gameData->level_filename()));
}

//#################### PRIVATE METHODS ####################
GUIComponent_Ptr GameState_LoadLevel::construct_display()
{
	GUIContainer<ExplicitLayout> *display = new GUIContainer<ExplicitLayout>;

	bf::path imagesDir = DirectoryFinder::instance().determine_images_directory();

	const Screen& screen = Screen::instance();
	int width = screen.dimensions().width();
	int height = screen.dimensions().height();
	display->layout().add(new Picture((imagesDir / "loading.png").file_string()), Extents(width/4, 0, width*3/4, width/8));

	// Determine the level name.
	std::string levelFilename = m_gameData->level_filename();
	size_t i = levelFilename.find_last_of("/\\");		// find the last slash (slanting either way) in the filename
	std::string levelName = i == std::string::npos ? levelFilename.substr(0) : levelFilename.substr(i+1);
	levelName = levelName.substr(0, levelName.length()-4);

	// Load the appropriate loading image for the level.
	Picture *loadingPicture;
	try { loadingPicture = new Picture((imagesDir / ("load-" + levelName + ".png")).file_string()); }
	catch(FileNotFoundException&) { loadingPicture = new Picture((imagesDir / "load-missing.png").file_string()); }

	display->layout().add(loadingPicture, Extents(50, width/8, width - 50, height - 50));

	return GUIComponent_Ptr(display);
}

}
