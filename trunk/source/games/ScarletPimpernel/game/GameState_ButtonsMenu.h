/***
 * ScarletPimpernel: GameState_ButtonsMenu.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_SCARLETPIMPERNEL_GAMESTATE_BUTTONSMENU
#define H_SCARLETPIMPERNEL_GAMESTATE_BUTTONSMENU

#include <vector>

#include <boost/optional.hpp>

#include <hesp/audio/SoundSystem.h>
#include "GameState.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
class Button;
typedef shared_ptr<class GameData> GameData_Ptr;

class GameState_ButtonsMenu : public GameState
{
	//#################### PROTECTED VARIABLES ####################
protected:
	GameData_Ptr m_gameData;
	boost::optional<SoundSystem::InstanceHandle> m_menuSoundInstance;

	//#################### CONSTRUCTORS ####################
public:
	explicit GameState_ButtonsMenu(const std::string& name, const GameData_Ptr& gameData);

	//#################### PRIVATE ABSTRACT METHODS ####################
private:
	virtual std::vector<Button*> menu_buttons() = 0;

	//#################### PUBLIC METHODS ####################
public:
	void enter();
	void execute();
	void leave();

	//#################### PROTECTED METHODS ####################
protected:
	GUIComponent_Ptr construct_menu(const std::vector<Button*> buttons);
};

}

#endif
