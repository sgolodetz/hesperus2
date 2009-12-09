/***
 * ScarletPimpernel: GameTransition_LevelLoaded.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_SCARLETPIMPERNEL_GAMETRANSITION_LEVELLOADED
#define H_SCARLETPIMPERNEL_GAMETRANSITION_LEVELLOADED

#include "GameFSM.h"

namespace hesp {

class GameTransition_LevelLoaded : public GameFSMTransition
{
	//#################### PRIVATE VARIABLES ####################
private:
	GameData_Ptr m_gameData;

	//#################### CONSTRUCTORS ####################
public:
	GameTransition_LevelLoaded(const GameData_Ptr& gameData);

	//#################### PUBLIC METHODS ####################
public:
	std::string execute();
	bool triggered() const;
};

}

#endif
