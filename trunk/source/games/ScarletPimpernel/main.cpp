/***
 * ScarletPimpernel: main.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include <cstdlib>
#include <iostream>

#include <hesp/datastructures/FiniteStateMachine.h>
#include <hesp/exceptions/Exception.h>
#include <hesp/io/util/DirectoryFinder.h>
#include "game/Game.h"
#include "game/GameData.h"
#include "game/GameState_InGameMenu.h"
#include "game/GameState_Level.h"
#include "game/GameState_LoadLevel.h"
#include "game/GameState_MainMenu.h"
#include "game/GameTransition_ExitLevel.h"
#include "game/GameTransition_LevelLoaded.h"
#include "game/GameTransition_NewGame.h"
#include "game/GameTransition_PauseLevel.h"
#include "game/GameTransition_ResumeLevel.h"
using namespace hesp;

int main(int argc, char* argv[])
try
{
	// Set the appropriate resources directory.
	DirectoryFinder& finder = DirectoryFinder::instance();
	finder.set_resources_directory(finder.determine_resources_directory_from_game("ScarletPimpernel"));

	// Construct the game state machine.
	FiniteStateMachine_Ptr gameFSM(new FiniteStateMachine);

	// Add game states.
	GameData_Ptr gameData(new GameData);

	GameState_InGameMenu_Ptr inGameMenuState(new GameState_InGameMenu(gameData));
	gameFSM->add_state(inGameMenuState);

	GameState_Level_Ptr levelState(new GameState_Level(gameData));
	gameFSM->add_state(levelState);

	gameFSM->add_state(FSMState_Ptr(new GameState_LoadLevel(gameData)));

	GameState_MainMenu_Ptr mainMenuState(new GameState_MainMenu(gameData));
	gameFSM->add_state(mainMenuState);

	// Add game transitions.
	gameFSM->add_transition(FSMTransition_Ptr(new GameTransition_ExitLevel(gameData, inGameMenuState)));
	gameFSM->add_transition(FSMTransition_Ptr(new GameTransition_LevelLoaded(gameData)));
	gameFSM->add_transition(FSMTransition_Ptr(new GameTransition_NewGame(gameData, mainMenuState)));
	gameFSM->add_transition(FSMTransition_Ptr(new GameTransition_PauseLevel(levelState)));
	gameFSM->add_transition(FSMTransition_Ptr(new GameTransition_ResumeLevel(inGameMenuState)));

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
