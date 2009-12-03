/***
 * hesperus: BitmapSaver.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_BITMAPSAVER
#define H_HESP_BITMAPSAVER

#include <iosfwd>
#include <string>

#include "Image.h"

namespace hesp {

/**
This struct provides saving functions for bitmaps of various bit depths (e.g. 24-bit).
*/
struct BitmapSaver
{
	//#################### PUBLIC METHODS ####################
	static void save_image24(const std::string& filename, const Image24_CPtr& image);
	static void save_streamed_image24(std::ostream& os, const Image24_CPtr& image);
};

}

#endif
