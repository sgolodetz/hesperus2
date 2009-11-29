/***
 * hesperus: PortalsFile.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "PortalsFile.h"

#include <fstream>

#include <boost/lexical_cast.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <hesp/exceptions/Exception.h>
#include <hesp/io/sections/PolygonsSection.h>

namespace hesp {

//#################### LOADING METHODS ####################
/**
Loads an empty leaf count and an array of portals from the specified file.

@param filename			The name of the portals file
@param emptyLeafCount	Used to return the empty leaf count to the caller
@param portals			Used to return the portals to the caller
*/
void PortalsFile::load(const std::string& filename, int& emptyLeafCount, std::vector<Portal_Ptr>& portals)
{
	std::ifstream is(filename.c_str(), std::ios_base::binary);
	if(is.fail()) throw Exception("Could not open " + filename + " for reading");

	std::string line;
	if(!LineIO::portable_getline(is, line)) throw Exception("The empty leaf count could not be read");
	try							{ emptyLeafCount = lexical_cast<int>(line); }
	catch(bad_lexical_cast&)	{ throw Exception("The empty leaf count was not an integer"); }

	PolygonsSection::load(is, "Portals", portals);
}

//#################### SAVING METHODS ####################
/**
Saves an empty leaf count and an array of portals to the specified file.

@param filename			The name of the portals file
@param emptyLeafCount	The empty leaf count
@param portals			The portals
*/
void PortalsFile::save(const std::string& filename, int emptyLeafCount, const std::vector<Portal_Ptr>& portals)
{
	std::ofstream os(filename.c_str(), std::ios_base::binary);
	if(os.fail()) throw Exception("Could not open " + filename + " for writing");

	os << emptyLeafCount << '\n';
	PolygonsSection::save(os, "Portals", portals);
}

}
