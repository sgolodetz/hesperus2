/***
 * hesperus: LightsFile.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LIGHTSFILE
#define H_HESP_LIGHTSFILE

#include <vector>

#include <hesp/level/lighting/Light.h>

namespace hesp {

struct LightsFile
{
	//#################### LOADING METHODS ####################
	static std::vector<Light> load(const std::string& filename);

	//#################### SAVING METHODS ####################
	static void save(const std::string& filename, const std::vector<Light>& lights);
};

}

#endif
