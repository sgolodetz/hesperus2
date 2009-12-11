/***
 * ScarletPimpernel: GameState_ButtonsMenu.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "GameState_ButtonsMenu.h"

#include <hesp/gui/Button.h>
#include <hesp/gui/Picture.h>
#include <hesp/gui/Screen.h>
#include <hesp/io/util/DirectoryFinder.h>
#include "GameData.h"

namespace bf = boost::filesystem;

namespace hesp {

//#################### CONSTRUCTORS ####################
GameState_ButtonsMenu::GameState_ButtonsMenu(const std::string& name, const GameData_Ptr& gameData)
:	GameState(name), m_gameData(gameData)
{}

//#################### PUBLIC METHODS ####################
void GameState_ButtonsMenu::execute()
{
	Screen::instance().handle_input(m_gameData->input());
}

//#################### PROTECTED METHODS ####################
GUIComponent_Ptr GameState_ButtonsMenu::construct_menu(const std::vector<Button*> buttons)
{
	GUIContainer<ExplicitLayout> *display = new GUIContainer<ExplicitLayout>;

	bf::path imagesDir = DirectoryFinder::instance().determine_images_directory();

	const Screen& screen = Screen::instance();
	int width = screen.dimensions().width();
	int height = screen.dimensions().height();

	// Add the title image.
	display->layout().add(new Picture((imagesDir / "title.png").file_string()), Extents(width/4, 0, width*3/4, width/8));

	// Add the backdrop image.
	int buttonCount = static_cast<int>(buttons.size());
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
		display->layout().add(buttons[i], Extents(buttonsLeft, buttonTop, buttonsRight, buttonTop+BUTTON_HEIGHT));
	}

	return GUIComponent_Ptr(display);
}

}
