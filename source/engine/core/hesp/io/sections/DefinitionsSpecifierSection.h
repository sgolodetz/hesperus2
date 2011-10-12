/***
 * hesperus: DefinitionsSpecifierSection.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_DEFINITIONSSPECIFIERSECTION
#define H_HESP_DEFINITIONSSPECIFIERSECTION

#include <iosfwd>
#include <string>

namespace hesp {

struct DefinitionsSpecifierSection
{
	//#################### LOADING METHODS ####################
	static std::string load(std::istream& is);

	//#################### SAVING METHODS ####################
	static void save(std::ostream& os, const std::string& definitionsFilename);
};

}

#endif
