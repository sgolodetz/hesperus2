/***
 * hesperus: DirectoryFinder.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_DIRECTORYFINDER
#define H_HESP_DIRECTORYFINDER

#include <boost/filesystem/operations.hpp>

namespace hesp {

class DirectoryFinder
{
	//#################### SINGLETON IMPLEMENTATION ####################
private:
	DirectoryFinder();
	DirectoryFinder(const DirectoryFinder&);
	DirectoryFinder& operator=(const DirectoryFinder&);
public:
	static DirectoryFinder& instance();

	//#################### PRIVATE VARIABLES ####################
private:
	boost::filesystem::path m_resourcesDir;

	//#################### PUBLIC METHODS ####################
public:
	boost::filesystem::path determine_definitions_directory() const;
	boost::filesystem::path determine_executable_location() const;
	boost::filesystem::path determine_resources_directory_from_tool(const std::string& game) const;
	void set_resources_directory(const boost::filesystem::path& resourcesDir);

	//#################### PRIVATE METHODS ####################
private:
	void check_resources_directory() const;
};

}

#endif
