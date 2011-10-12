/***
 * test-FMOD-4.28.02: main.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <iostream>

#include <fmod.hpp>
#include <fmod_errors.h>

void check(FMOD_RESULT result)
{
	if(result != FMOD_OK)
	{
		std::cout << FMOD_ErrorString(result) << '\n';
	}
}

int main()
{
	// Create the system.
	FMOD::System *system;
	FMOD_RESULT result = FMOD::System_Create(&system);
	check(result);

	// Initialise the system.
	result = system->init(100, FMOD_INIT_3D_RIGHTHANDED, NULL);
	check(result);

	// Release the system.
	result = system->release();
	check(result);

	return 0;
}
