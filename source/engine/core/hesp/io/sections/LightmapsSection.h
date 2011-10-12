/***
 * hesperus: LightmapsSection.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LIGHTMAPSSECTION
#define H_HESP_LIGHTMAPSSECTION

#include <iosfwd>
#include <vector>

#include <hesp/images/Image.h>

namespace hesp {

struct LightmapsSection
{
	//#################### LOADING METHODS ####################
	static std::vector<Image24_Ptr> load(std::istream& is);

	//#################### SAVING METHODS ####################
	static void save(std::ostream& os, const std::vector<Image24_Ptr>& lightmaps);
};

}

#endif
