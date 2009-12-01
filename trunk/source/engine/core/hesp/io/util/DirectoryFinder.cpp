/***
 * hesperus: DirectoryFinder.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "DirectoryFinder.h"
namespace bf = boost::filesystem;

#if defined(_WIN32)
	#include <windows.h>
#elif defined(__linux__)
	#include <unistd.h>
#endif

namespace hesp {

bf::path determine_executable_location()
{
#if defined(_WIN32)
	std::string s;
	s.resize(512);
	::GetModuleFileName(NULL, &s[0], 512);
	return s;
#elif defined(__linux__)
	#error NYI
#else
	#error Can't yet determine the executable location on this platform
#endif
}

}
