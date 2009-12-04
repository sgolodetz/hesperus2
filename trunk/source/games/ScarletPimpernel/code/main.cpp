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

int main(int argc, char* argv[])
try
{
	// Set the appropriate resources directory.
	DirectoryFinder& finder = DirectoryFinder::instance();
	finder.set_resources_directory(finder.determine_resources_directory_from_game("ScarletPimpernel"));

	Game game;
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
