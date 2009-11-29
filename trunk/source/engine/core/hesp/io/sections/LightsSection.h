/***
 * hesperus: LightsSection.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LIGHTSSECTION
#define H_HESP_LIGHTSSECTION

#include <iosfwd>
#include <vector>

#include <hesp/level/lighting/Light.h>

namespace hesp {

struct LightsSection
{
	//#################### LOADING METHODS ####################
	static std::vector<Light> load(std::istream& is);

	//#################### SAVING METHODS ####################
	static void save(std::ostream& os, const std::vector<Light>& lights);
};

}

#endif
