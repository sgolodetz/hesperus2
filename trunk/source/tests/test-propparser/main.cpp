/***
 * test-propparser: main.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <iostream>

#include <PropSimplifier.h>

int main()
{
	std::cout << propparser::simplify_property("\"Blah\"") << '\n';
	return 0;
}
