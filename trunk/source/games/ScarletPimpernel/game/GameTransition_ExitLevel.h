/***
 * ScarletPimpernel: GameTransition_ExitLevel.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_SCARLETPIMPERNEL_GAMETRANSITION_EXITLEVEL
#define H_SCARLETPIMPERNEL_GAMETRANSITION_EXITLEVEL

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <hesp/datastructures/FSMTransition.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class GameData> GameData_Ptr;
typedef shared_ptr<class GameState_InGameMenu> GameState_InGameMenu_Ptr;

class GameTransition_ExitLevel : public FSMTransition
{
	//#################### PRIVATE VARIABLES ####################
private:
	GameData_Ptr m_gameData;
	GameState_InGameMenu_Ptr m_inGameMenuState;

	//#################### CONSTRUCTORS ####################
public:
	explicit GameTransition_ExitLevel(const GameData_Ptr& gameData, const GameState_InGameMenu_Ptr& inGameMenuState);

	//#################### PUBLIC METHODS ####################
public:
	void execute();
	bool triggered() const;
};

}

#endif
