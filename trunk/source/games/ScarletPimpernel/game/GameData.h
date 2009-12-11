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

class GameData
{
	//#################### PRIVATE VARIABLES ####################
private:
	InputState m_input;
	Level_Ptr m_level;
	std::string m_levelFilename;
	int m_milliseconds;
	bool m_quitRequested;
	SoundSystem m_soundSystem;

	//#################### CONSTRUCTORS ####################
public:
	GameData();

	//#################### PUBLIC METHODS ####################
public:
	InputState& input();
	Level_Ptr level();
	const std::string& level_filename();
	int milliseconds() const;
	bool quit_requested() const;
	void set_level(const Level_Ptr& level);
	void set_level_filename(const std::string& levelFilename);
	void set_milliseconds(int milliseconds);
	void set_quit_requested();
	SoundSystem& sound_system();
};

//#################### TYPEDEFS ####################
typedef shared_ptr<GameData> GameData_Ptr;
typedef shared_ptr<const GameData> GameData_CPtr;

}

#endif
