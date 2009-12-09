/***
 * ScarletPimpernel: main.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include <cstdlib>
#include <iostream>

#include <hesp/exceptions/Exception.h>
#include <hesp/io/util/DirectoryFinder.h>
#include "game/Game.h"
#include "game/GameFSM.h"
#include "game/GameState_Level.h"
#include "game/GameState_LoadLevel.h"
#include "game/GameState_MainMenu.h"
#include "game/GameTransition_LevelLoaded.h"
#include "game/GameTransition_NewGame.h"
using namespace hesp;

int main(int argc, char* argv[])
try
{
	// Set the appropriate resources directory.
	DirectoryFinder& finder = DirectoryFinder::instance();
	finder.set_resources_directory(finder.determine_resources_directory_from_game("ScarletPimpernel"));

	// Construct the game state machine.
	GameFSM_Ptr gameFSM(new GameFSM);

	// Add game states.
	GameData_Ptr gameData(new GameData);
	gameFSM->add_state(GameFSMState_Ptr(new GameState_Level(gameData)));
	gameFSM->add_state(GameFSMState_Ptr(new GameState_LoadLevel(gameData)));
	GameState_MainMenu_Ptr mainMenuState(new GameState_MainMenu(gameData));
	gameFSM->add_state(mainMenuState);

	// Add game transitions.
	gameFSM->add_transition(GameFSMTransition_Ptr(new GameTransition_LevelLoaded(gameData)));
	gameFSM->add_transition(GameFSMTransition_Ptr(new GameTransition_NewGame(gameData, mainMenuState)));

	// Construct and run the game.
	Game game(gameFSM, "MainMenu", gameData);
	game.run();

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
