/***
 * hesperus: DirectoryFinder.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "DirectoryFinder.h"

#include <vector>

#include <hesp/exceptions/Exception.h>

namespace bf = boost::filesystem;

#if defined(_WIN32)
	#include <windows.h>
#elif defined(__linux__)
	#include <sstream>
	#include <unistd.h>
#endif

namespace hesp {

bf::path determine_executable_location()
{
	const int BUFFER_SIZE = 512;
	std::vector<char> buffer(BUFFER_SIZE+1);

#if defined(_WIN32)
	::GetModuleFileName(NULL, &buffer[0], BUFFER_SIZE);
#elif defined(__linux__)
	// Get the process ID.
	int pid = getpid();

	// Construct a path to the symbolic link pointing to the process executable.
	// This is at /proc/<pid>/exe on Linux systems (we hope).
	std::ostringstream oss;
	oss << "/proc/" << pid << "/exe";
	std::string link = oss.str();

	// Read the contents of the link.
	int count = readlink(link.c_str(), &buffer[0], BUFFER_SIZE);
	if(count == -1) throw Exception("Could not read symbolic link");
	buffer[count] = '\0';
#else
	#error Cannot yet determine the executable location on this platform
#endif

	std::string s = &buffer[0];
	return s;
}

}
