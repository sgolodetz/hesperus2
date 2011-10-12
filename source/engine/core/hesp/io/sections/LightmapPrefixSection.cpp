/***
 * hesperus: LightmapPrefixSection.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "LightmapPrefixSection.h"

#include <iostream>

#include <hesp/io/util/LineIO.h>

namespace hesp {

//#################### LOADING METHODS ####################
/**
Loads a lightmap prefix from the specified std::istream.

@param is			The std::istream
@return				The lightmap prefix
@throws Exception	If EOF is encountered whilst trying to read the lightmap prefix
*/
std::string LightmapPrefixSection::load(std::istream& is)
{
	std::string lightmapPrefix;

	LineIO::read_checked_line(is, "LightmapPrefix");
	LineIO::read_checked_line(is, "{");
	LineIO::read_line(is, lightmapPrefix, "lightmap prefix");
	LineIO::read_checked_line(is, "}");

	return lightmapPrefix;
}

//#################### SAVING METHODS ####################
/**
Saves a lightmap prefix to the specified std::ostream.

@param os	The std::ostream
@param		The lightmap prefix
*/
void LightmapPrefixSection::save(std::ostream& os, const std::string& lightmapPrefix)
{
	os << "LightmapPrefix\n";
	os << "{\n";
	os << lightmapPrefix << '\n';
	os << "}\n";
}

}
