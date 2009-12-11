/***
 * ScarletPimpernel: GameTransition_PauseLevel.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_SCARLETPIMPERNEL_GAMETRANSITION_PAUSELEVEL
#define H_SCARLETPIMPERNEL_GAMETRANSITION_PAUSELEVEL

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <hesp/datastructures/FSMTransition.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class GameState_Level> GameState_Level_Ptr;

class GameTransition_PauseLevel : public FSMTransition
{
	//#################### PRIVATE VARIABLES ####################
private:
	GameState_Level_Ptr m_levelState;

	//#################### CONSTRUCTORS ####################
public:
	explicit GameTransition_PauseLevel(const GameState_Level_Ptr& levelState);

	//#################### PUBLIC METHODS ####################
public:
	bool triggered() const;
};

}

#endif
