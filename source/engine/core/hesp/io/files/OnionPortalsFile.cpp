/***
 * hesperus: OnionPortalsFile.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "OnionPortalsFile.h"

#include <fstream>

#include <hesp/io/sections/PolygonsSection.h>

namespace hesp {

//#################### SAVING METHODS ####################
/**
Loads an array of onion portals from the specified file.

@param filename		The name of the onion portals file
@return				The onion portals
*/
std::vector<OnionPortal_Ptr> OnionPortalsFile::load(const std::string& filename)
{
	std::ifstream is(filename.c_str(), std::ios_base::binary);
	if(is.fail()) throw Exception("Could not open " + filename + " for reading");

	std::vector<OnionPortal_Ptr> portals;
	PolygonsSection::load(is, "OnionPortals", portals);
	return portals;
}

//#################### SAVING METHODS ####################
/**
Saves an array of onion portals to the specified file.

@param filename		The name of the onion portals file
@param portals		The onion portals
*/
void OnionPortalsFile::save(const std::string& filename, const std::vector<OnionPortal_Ptr>& portals)
{
	std::ofstream os(filename.c_str(), std::ios_base::binary);
	if(os.fail()) throw Exception("Could not open " + filename + " for writing");
	PolygonsSection::save(os, "OnionPortals", portals);
}

}
