/***
 * ScarletPimpernel: GameTransition_LevelLoaded.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_SCARLETPIMPERNEL_GAMETRANSITION_LEVELLOADED
#define H_SCARLETPIMPERNEL_GAMETRANSITION_LEVELLOADED

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <hesp/datastructures/FSMTransition.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<struct GameData> GameData_Ptr;

class GameTransition_LevelLoaded : public FSMTransition
{
	//#################### PRIVATE VARIABLES ####################
private:
	GameData_Ptr m_gameData;

	//#################### CONSTRUCTORS ####################
public:
	GameTransition_LevelLoaded(const GameData_Ptr& gameData);

	//#################### PUBLIC METHODS ####################
public:
	void execute();
	bool triggered() const;
};

}

#endif
