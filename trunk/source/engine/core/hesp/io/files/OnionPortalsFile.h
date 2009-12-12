/***
 * hesperus: OnionPortalsFile.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ONIONPORTALSFILE
#define H_HESP_ONIONPORTALSFILE

#include <string>
#include <vector>

#include <hesp/portals/OnionPortal.h>

namespace hesp {

struct OnionPortalsFile
{
	//#################### LOADING METHODS ####################
	static std::vector<OnionPortal_Ptr> load(const std::string& filename);

	//#################### SAVING METHODS ####################
	static void save(const std::string& filename, const std::vector<OnionPortal_Ptr>& portals);
};

}

#endif
