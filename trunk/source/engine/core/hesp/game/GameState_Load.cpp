/***
 * hesperus: GameState_Load.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "GameState_Load.h"

#include <hesp/exceptions/FileNotFoundException.h>
#include <hesp/gui/ExplicitLayout.h>
#include <hesp/gui/Picture.h>
#include <hesp/gui/Screen.h>
#include <hesp/io/util/DirectoryFinder.h>
#include "GameState_Level.h"
namespace bf = boost::filesystem;

namespace hesp {

//#################### CONSTRUCTORS ####################
GameState_Load::GameState_Load(const std::string& levelFilename)
:	m_levelFilename(levelFilename), m_firstTime(true)
{}

//#################### PUBLIC METHODS ####################
void GameState_Load::enter()
{
	set_display(construct_display());
}

void GameState_Load::leave()
{
	// Nothing to do
}

GameState_Ptr GameState_Load::update(int milliseconds, InputState& input)
{
	// Ensure that the loading screen's been rendered before we try and load the level (render happens after updating).
	if(m_firstTime)
	{
		m_firstTime = false;
		return GameState_Ptr();
	}

	return GameState_Ptr(new GameState_Level(m_levelFilename));
}

//#################### PRIVATE METHODS ####################
GUIComponent_Ptr GameState_Load::construct_display()
{
	GUIContainer<ExplicitLayout> *display = new GUIContainer<ExplicitLayout>;

	bf::path imagesDir = DirectoryFinder::instance().determine_images_directory();

	const Screen& screen = Screen::instance();
	int width = screen.dimensions().width();
	int height = screen.dimensions().height();
	display->layout().add(new Picture((imagesDir / "loading.png").file_string()), Extents(width/4, 0, width*3/4, width/8));

	// Determine the level name.
	size_t i = m_levelFilename.find_last_of("/\\");		// find the last slash (slanting either way) in the filename
	std::string levelName = i == std::string::npos ? m_levelFilename.substr(0) : m_levelFilename.substr(i+1);
	levelName = levelName.substr(0, levelName.length()-4);

	// Load the appropriate loading image for the level.
	Picture *loadingPicture;
	try { loadingPicture = new Picture((imagesDir / ("load-" + levelName + ".png")).file_string()); }
	catch(FileNotFoundException&) { loadingPicture = new Picture((imagesDir / "load-missing.png").file_string()); }

	display->layout().add(loadingPicture, Extents(50, width/8, width - 50, height - 50));

	return GUIComponent_Ptr(display);
}

}
