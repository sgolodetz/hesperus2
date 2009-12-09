/***
 * ScarletPimpernel: main.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include <cstdlib>
#include <iostream>

#include <hesp/exceptions/Exception.h>
#include <hesp/game/Game.h>
#include <hesp/io/util/DirectoryFinder.h>
using namespace hesp;

#if 1
#include "game/GameFSM.h"
#include "game/GameRunner.h"
#include "game/GameState_LoadLevel.h"
#include "game/GameState_MainMenu.h"
#include "game/GameState_Play.h"
#include "game/GameTransition_LevelLoaded.h"
#include "game/GameTransition_NewGame.h"
#endif

int main(int argc, char* argv[])
try
{
	// Set the appropriate resources directory.
	DirectoryFinder& finder = DirectoryFinder::instance();
	finder.set_resources_directory(finder.determine_resources_directory_from_game("ScarletPimpernel"));

#if 1
	GameFSM_Ptr gameFSM(new GameFSM);

	// Add game states.
	GameData_Ptr gameData(new GameData);

	gameFSM->add_state(GameFSMState_Ptr(new GameState_LoadLevel(gameData)));

	GameState_MainMenu_Ptr mainMenuState(new GameState_MainMenu(gameData));
	gameFSM->add_state(mainMenuState);

	gameFSM->add_state(GameFSMState_Ptr(new GameState_Play(gameData)));

	// Add game transitions.
	gameFSM->add_transition(GameFSMTransition_Ptr(new GameTransition_LevelLoaded(gameData)));
	gameFSM->add_transition(GameFSMTransition_Ptr(new GameTransition_NewGame(gameData, mainMenuState)));

	// Construct the game runner.
	GameRunner gameRunner(gameFSM, "MainMenu", gameData);

	// Execute the game runner.
	gameRunner.run();
#endif

#if 0
	Game game;
	game.run();
#endif

	return 0;
}
catch(Exception& e)
{
	std::cerr << "Error: " << e.cause() << std::endl;
	return EXIT_FAILURE;
}
catch(...)
{
	std::cerr << "Error: Something strange and unexpected happened" << std::endl;
	return EXIT_FAILURE;
}
