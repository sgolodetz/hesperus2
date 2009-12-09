/***
 * ScarletPimpernel: GameData.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_SCARLETPIMPERNEL_GAMEDATA
#define H_SCARLETPIMPERNEL_GAMEDATA

#include <string>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <hesp/audio/SoundSystem.h>
#include <hesp/input/InputState.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class Level> Level_Ptr;

struct GameData
{
	//#################### PUBLIC VARIABLES ####################
	InputState m_input;
	Level_Ptr m_level;
	std::string m_levelFilename;
	int m_milliseconds;
	bool m_quitRequested;
	SoundSystem m_soundSystem;

	//#################### CONSTRUCTORS ####################
	GameData();
};

//#################### TYPEDEFS ####################
typedef shared_ptr<GameData> GameData_Ptr;
typedef shared_ptr<const GameData> GameData_CPtr;

}

#endif
