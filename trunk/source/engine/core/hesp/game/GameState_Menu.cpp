/***
 * hesperus: GameState_Menu.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "GameState_Menu.h"

#include <boost/bind.hpp>

#include <hesp/audio/SoundSystem.h>
#include <hesp/gui/Button.h>
#include <hesp/gui/ExplicitLayout.h>
#include <hesp/gui/Picture.h>
#include <hesp/gui/Screen.h>
#include <hesp/io/util/DirectoryFinder.h>
#include "GameState_Load.h"

namespace bf = boost::filesystem;

namespace hesp {

//#################### CONSTRUCTORS ####################
GameState_Menu::GameState_Menu(const std::string& menu, const std::string& initialLevelFilename, SoundSystem& soundSystem)
:	m_menu(menu), m_initialLevelFilename(initialLevelFilename), m_soundSystem(soundSystem)
{}

//#################### PUBLIC METHODS ####################
void GameState_Menu::enter()
{
	// TODO: Construct the appropriate menu based on the parameter to the constructor.
	set_display(construct_buttons_menu(menu_buttons_main()));

	bf::path audioDir = DirectoryFinder::instance().determine_audio_directory();
	m_soundSystem.create_sound("menu", (audioDir / "menu.mid").file_string(), SF_STREAM | SF_2D | SF_LOOP);
	m_soundSystem.play_sound("menu");
}

void GameState_Menu::leave()
{
	m_soundSystem.destroy_sound("menu");
}

GameState_Ptr GameState_Menu::update(int milliseconds, InputState& input)
{
	Screen& screen = Screen::instance();
	screen.handle_input(input);
	return m_newState;
}

//#################### PRIVATE METHODS ####################
GUIComponent_Ptr GameState_Menu::construct_buttons_menu(const std::vector<Button*> menuButtons)
{
	GUIContainer<ExplicitLayout> *display = new GUIContainer<ExplicitLayout>;

	bf::path imagesDir = DirectoryFinder::instance().determine_images_directory();

	const Screen& screen = Screen::instance();
	int width = screen.dimensions().width();
	int height = screen.dimensions().height();

	// Add the title image.
	display->layout().add(new Picture((imagesDir / "title.png").file_string()), Extents(width/4, 0, width*3/4, width/8));

	// Add the backdrop image.
	int buttonCount = static_cast<int>(menuButtons.size());
	const int BUTTON_HEIGHT = height/7;
	const int BUTTON_SPACING = height/14;						// space between adjacent buttons
	const int BUTTON_STRIDE = BUTTON_HEIGHT + BUTTON_SPACING;	// vertical distance between the centres of adjacent buttons
	int buttonsLeft = width*3/10;
	int buttonsRight = width*7/10;
	int buttonsTop = width/8 + 2*BUTTON_SPACING;
	int buttonsBottom = buttonsTop + buttonCount * BUTTON_STRIDE - BUTTON_SPACING;

	Extents backdropExtents(buttonsLeft - BUTTON_SPACING, buttonsTop - BUTTON_SPACING,
							buttonsRight + BUTTON_SPACING, buttonsBottom + BUTTON_SPACING);
	display->layout().add(new Picture((imagesDir / "menu-backdrop.png").file_string()), backdropExtents);

	// Add the right-facing Percy image.
	int prLeft = BUTTON_SPACING;
	int prRight = buttonsLeft - 2*BUTTON_SPACING;
	int percyTop = buttonsTop - BUTTON_SPACING;
	int percyBottom = percyTop + 2*(prRight-prLeft);
	Extents prExtents(prLeft, percyTop, prRight, percyBottom);
	display->layout().add(new Picture((imagesDir / "menu-percyright.png").file_string()), prExtents);

	// Add the left-facing Percy image.
	int plRight = width - BUTTON_SPACING;
	int plLeft = buttonsRight + 2*BUTTON_SPACING;
	Extents plExtents(plLeft, percyTop, plRight, percyBottom);
	display->layout().add(new Picture((imagesDir / "menu-percyleft.png").file_string()), plExtents);

	// Add the pimpernel images.
	display->layout().add(new Picture((imagesDir / "menu-pimpernel.png").file_string()), Extents(width/16, 0, 3*width/16, width/8));
	display->layout().add(new Picture((imagesDir / "menu-pimpernel.png").file_string()), Extents(13*width/16, 0, 15*width/16, width/8));

	// Add the FMOD copyright notice.
	display->layout().add(new Picture((imagesDir / "fmod-copyright.png").file_string()), Extents(0, height-width/16, width, height));

	// Add the buttons.
	for(int i=0; i<buttonCount; ++i)
	{
		int buttonTop = buttonsTop + i*BUTTON_STRIDE;
		display->layout().add(menuButtons[i], Extents(buttonsLeft, buttonTop, buttonsRight, buttonTop+BUTTON_HEIGHT));
	}

	return GUIComponent_Ptr(display);
}

std::vector<Button*> GameState_Menu::menu_buttons_main()
{
	std::vector<Button*> ret;
	bf::path imagesDir = DirectoryFinder::instance().determine_images_directory();

	ret.push_back(new Button((imagesDir / "buttons-newgame-inactive.png").file_string(),
							 (imagesDir / "buttons-newgame-active.png").file_string(),
							 boost::none, boost::bind(&GameState_Menu::on_released_NewGame, this)));

	ret.push_back(new Button((imagesDir / "buttons-exitgame-inactive.png").file_string(),
							 (imagesDir / "buttons-exitgame-active.png").file_string(),
							 boost::none, boost::bind(&GameState_Menu::on_released_ExitGame, this)));

	return ret;
}

//#################### BUTTON HANDLERS ####################
void GameState_Menu::on_released_ExitGame()
{
	set_quit_flag();
}

void GameState_Menu::on_released_NewGame()
{
	m_newState.reset(new GameState_Load(m_initialLevelFilename));
}

}
