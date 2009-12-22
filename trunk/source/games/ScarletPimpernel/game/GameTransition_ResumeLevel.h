/***
 * ScarletPimpernel: GameTransition_ResumeLevel.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_SCARLETPIMPERNEL_GAMETRANSITION_RESUMELEVEL
#define H_SCARLETPIMPERNEL_GAMETRANSITION_RESUMELEVEL

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <hesp/statemachines/FSMTransition.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class GameState_InGameMenu> GameState_InGameMenu_Ptr;

class GameTransition_ResumeLevel : public FSMTransition
{
	//#################### PRIVATE VARIABLES ####################
private:
	GameState_InGameMenu_Ptr m_inGameMenuState;

	//#################### CONSTRUCTORS ####################
public:
	explicit GameTransition_ResumeLevel(const GameState_InGameMenu_Ptr& inGameMenuState);

	//#################### PUBLIC METHODS ####################
public:
	bool triggered() const;
};

}

#endif
