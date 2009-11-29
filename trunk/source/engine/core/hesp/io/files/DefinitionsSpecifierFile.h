/***
 * hesperus: DefinitionsSpecifierFile.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_DEFINITIONSSPECIFIERFILE
#define H_HESP_DEFINITIONSSPECIFIERFILE

#include <string>

namespace hesp {

struct DefinitionsSpecifierFile
{
	//#################### LOADING METHODS ####################
	static std::string load(const std::string& filename);

	//#################### SAVING METHODS ####################
	static void save(const std::string& filename, const std::string& definitionsFilename);
};

}

#endif
