/***
 * test-findexe: main.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <iostream>

#include <hesp/io/util/DirectoryFinder.h>
using namespace hesp;

namespace bf = boost::filesystem;

int main()
{
	bf::path path = determine_executable_location();
	std::cout << path.file_string() << '\n';
	return 0;
}
