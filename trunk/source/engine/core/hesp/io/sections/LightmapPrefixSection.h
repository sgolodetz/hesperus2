/***
 * hesperus: LightmapPrefixSection.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LIGHTMAPPREFIXSECTION
#define H_HESP_LIGHTMAPPREFIXSECTION

#include <iosfwd>
#include <string>

namespace hesp {

struct LightmapPrefixSection
{
	//#################### LOADING METHODS ####################
	static std::string load(std::istream& is);

	//#################### SAVING METHODS ####################
	static void save(std::ostream& os, const std::string& lightmapPrefix);
};

}

#endif
