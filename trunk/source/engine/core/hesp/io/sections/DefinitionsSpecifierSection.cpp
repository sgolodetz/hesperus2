/***
 * hesperus: DefinitionsSpecifierSection.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "DefinitionsSpecifierSection.h"

#include <hesp/io/util/LineIO.h>

namespace hesp {

//#################### LOADING METHODS ####################
std::string DefinitionsSpecifierSection::load(std::istream& is)
{
	std::string definitionsFilename;

	LineIO::read_checked_line(is, "DefinitionsSpecifier");
	LineIO::read_checked_line(is, "{");
	LineIO::read_line(is, definitionsFilename, "definitions filename");
	LineIO::read_checked_line(is, "}");

	return definitionsFilename;
}

//#################### SAVING METHODS ####################
void DefinitionsSpecifierSection::save(std::ostream& os, const std::string& definitionsFilename)
{
	os << "DefinitionsSpecifier\n";
	os << "{\n";
	os << definitionsFilename << '\n';
	os << "}\n";
}

}
