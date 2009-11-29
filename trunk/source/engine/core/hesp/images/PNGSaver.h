/***
 * hesperus: PNGSaver.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_PNGSAVER
#define H_HESP_PNGSAVER

#include <iosfwd>
#include <string>
#include <vector>

#include "Image.h"

namespace hesp {

/**
This class provides saving functions for PNGs of various bit depths (e.g. 24-bit).
*/
class PNGSaver
{
	//#################### PUBLIC METHODS ####################
public:
	static void save_image24(const std::string& filename, const Image24_CPtr& image);
	static void save_streamed_image24(std::ostream& os, const Image24_CPtr& image);

	//#################### PRIVATE METHODS ####################
private:
	static void encode_png(const Image24_CPtr& image, std::vector<unsigned char>& buffer);
};

}

#endif
