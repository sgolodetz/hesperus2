/***
 * ScarletPimpernel: GameState_MainMenu.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "GameState_MainMenu.h"

#include <boost/bind.hpp>

#include <hesp/gui/Button.h>
#include <hesp/gui/Picture.h>
#include <hesp/gui/Screen.h>
#include <hesp/io/util/DirectoryFinder.h>

namespace bf = boost::filesystem;

namespace hesp {

//#################### CONSTRUCTORS ####################
GameState_MainMenu::GameState_MainMenu(const GameData_Ptr& gameData)
:	GameFSMState("MainMenu"), m_gameData(gameData)
{}

//#################### PUBLIC METHODS ####################
void GameState_MainMenu::enter()
{
	// Reset all the button flags.
	m_newGamePressed = false;

	set_display(construct_buttons_menu(menu_buttons()));

#ifndef __linux__
	// Load and play the menu music (except on Linux, which has a problem with MIDI files for some reason).
	bf::path audioDir = DirectoryFinder::instance().determine_audio_directory();
	m_gameData->m_soundSystem.create_sound("menu", (audioDir / "menu.mid").file_string(), SF_STREAM | SF_2D | SF_LOOP);
	m_gameData->m_soundSystem.play_sound("menu");
#endif
}

void GameState_MainMenu::execute()
{
	Screen::instance().handle_input(m_gameData->m_input);
}

void GameState_MainMenu::leave()
{
#ifndef __linux__
	m_gameData->m_soundSystem.destroy_sound("menu");
#endif
}

bool GameState_MainMenu::new_game_pressed() const
{
	return m_newGamePressed;
}

//#################### PRIVATE METHODS ####################
GUIComponent_Ptr GameState_MainMenu::construct_buttons_menu(const std::vector<Button*> menuButtons)
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

std::vector<Button*> GameState_MainMenu::menu_buttons()
{
	std::vector<Button*> ret;
	bf::path imagesDir = DirectoryFinder::instance().determine_images_directory();

	ret.push_back(new Button((imagesDir / "buttons-newgame-inactive.png").file_string(),
							 (imagesDir / "buttons-newgame-active.png").file_string(),
							 boost::none, boost::bind(&GameState_MainMenu::on_released_NewGame, this)));

	ret.push_back(new Button((imagesDir / "buttons-exitgame-inactive.png").file_string(),
							 (imagesDir / "buttons-exitgame-active.png").file_string(),
							 boost::none, boost::bind(&GameState_MainMenu::on_released_ExitGame, this)));

	return ret;
}

//#################### BUTTON HANDLERS ####################
void GameState_MainMenu::on_released_ExitGame()
{
	m_gameData->m_quitRequested = true;
}

void GameState_MainMenu::on_released_NewGame()
{
	m_newGamePressed = true;
}

}
