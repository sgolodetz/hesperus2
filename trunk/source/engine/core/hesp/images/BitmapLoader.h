/***
 * hesperus: BitmapLoader.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_BITMAPLOADER
#define H_HESP_BITMAPLOADER

#include <iosfwd>
#include <string>

#include "Image.h"

namespace hesp {

/**
This struct provides loading functions for bitmaps of various bit depths (e.g. 24-bit).
*/
struct BitmapLoader
{
	//#################### PUBLIC METHODS ####################
	static Image24_Ptr load_image24(const std::string& filename);
	static Image24_Ptr load_streamed_image24(std::istream& is);
};

}

#endif
