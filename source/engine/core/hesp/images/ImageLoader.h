/***
 * hesperus: ImageLoader.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_IMAGELOADER
#define H_HESP_IMAGELOADER

#include <map>
#include <string>

#include "Image.h"

namespace hesp {

/**
This class provides generic image loading functions (image type is deduced from the file extension).
*/
class ImageLoader
{
	//#################### PUBLIC METHODS ####################
public:
	static Image24_Ptr load_image24(const std::string& filename);
	static Image32_Ptr load_image32(const std::string& filename);

	//#################### PRIVATE METHODS ####################
private:
	static std::string determine_extension(const std::string& filename);
};

}

#endif
