/***
 * ScarletPimpernel: GameTransition_NewGame.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_SCARLETPIMPERNEL_GAMETRANSITION_NEWGAME
#define H_SCARLETPIMPERNEL_GAMETRANSITION_NEWGAME

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <hesp/statemachines/FSMTransition.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class GameData> GameData_Ptr;
typedef shared_ptr<class GameState_MainMenu> GameState_MainMenu_Ptr;

class GameTransition_NewGame : public FSMTransition
{
	//#################### PRIVATE VARIABLES ####################
private:
	GameData_Ptr m_gameData;
	GameState_MainMenu_Ptr m_mainMenuState;

	//#################### CONSTRUCTORS ####################
public:
	GameTransition_NewGame(const GameData_Ptr& gameData, const GameState_MainMenu_Ptr& mainMenuState);

	//#################### PUBLIC METHODS ####################
public:
	void execute();
	bool triggered() const;
};

}

#endif
