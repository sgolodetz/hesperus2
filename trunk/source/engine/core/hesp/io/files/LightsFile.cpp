/***
 * hesperus: LightsFile.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "LightsFile.h"

#include <fstream>

#include <hesp/exceptions/Exception.h>
#include <hesp/io/sections/LightsSection.h>

namespace hesp {

//#################### LOADING METHODS ####################
/**
Loads an array of lights from the specified file.

@param filename	The name of the file containing the list of lights
@return			The array of lights
*/
std::vector<Light> LightsFile::load(const std::string& filename)
{
	std::ifstream is(filename.c_str(), std::ios_base::binary);
	if(is.fail()) throw Exception("Could not open " + filename + " for reading");
	return LightsSection::load(is);
}

//#################### SAVING METHODS ####################
void LightsFile::save(const std::string& filename, const std::vector<Light>& lights)
{
	std::ofstream os(filename.c_str(), std::ios_base::binary);
	if(os.fail()) throw Exception("Could not open " + filename + " for writing");
	LightsSection::save(os, lights);
}

}
