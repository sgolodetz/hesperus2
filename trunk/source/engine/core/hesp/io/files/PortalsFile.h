/***
 * hesperus: PortalsFile.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_PORTALSFILE
#define H_HESP_PORTALSFILE

#include <string>
#include <vector>

#include <hesp/level/portals/Portal.h>

namespace hesp {

struct PortalsFile
{
	//#################### LOADING METHODS ####################
	static void load(const std::string& filename, int& emptyLeafCount, std::vector<Portal_Ptr>& portals);

	//#################### SAVING METHODS ####################
	static void save(const std::string& filename, int emptyLeafCount, const std::vector<Portal_Ptr>& portals);
};

}

#endif
