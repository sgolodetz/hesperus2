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

//#################### SINGLETON IMPLEMENTATION ####################
DirectoryFinder::DirectoryFinder() {}

DirectoryFinder& DirectoryFinder::instance()
{
	static DirectoryFinder s_instance;
	return s_instance;
}

//#################### PUBLIC METHODS ####################
bf::path DirectoryFinder::determine_definitions_directory() const
{
	check_resources_directory();
	return m_resourcesDir / "definitions/";
}

bf::path DirectoryFinder::determine_executable_location() const
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

bf::path DirectoryFinder::determine_models_directory() const
{
	check_resources_directory();
	return m_resourcesDir / "models/";
}

bf::path DirectoryFinder::determine_profiles_directory() const
{
	check_resources_directory();
	return m_resourcesDir / "profiles/";
}

bf::path DirectoryFinder::determine_resources_directory_from_tool(const std::string& game) const
{
	bf::path p = determine_executable_location();	// hesperus2/<build|install>/bin/engine/tools/<?>.exe
	for(int i=0; i<5; ++i) p = p.parent_path();		// -> hesperus2
	p = p / "resources" / game;						// -> hesperus2/resources/<game>

	if(bf::exists(p)) return p;
	else throw Exception("Resources directory does not exist: " + p.file_string());
}

bf::path DirectoryFinder::determine_scripts_directory() const
{
	check_resources_directory();
	return m_resourcesDir / "scripts/";
}

bf::path DirectoryFinder::determine_sprites_directory() const
{
	check_resources_directory();
	return m_resourcesDir / "sprites/";
}

bf::path DirectoryFinder::determine_textures_directory() const
{
	check_resources_directory();
	return m_resourcesDir / "textures/";
}

void DirectoryFinder::set_resources_directory(const bf::path& resourcesDir)
{
	m_resourcesDir = resourcesDir;
}

//#################### PRIVATE METHODS ####################
void DirectoryFinder::check_resources_directory() const
{
	if(m_resourcesDir.empty()) throw Exception("Resources directory not set");
}

}
