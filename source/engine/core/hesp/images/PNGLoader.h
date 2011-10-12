/***
 * hesperus: PNGLoader.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_PNGLOADER
#define H_HESP_PNGLOADER

#include <istream>
#include <string>
#include <vector>

#include "Image.h"

namespace hesp {

/**
This class provides loading functions for PNGs of various bit depths (e.g. 24-bit).
*/
class PNGLoader
{
	//#################### PUBLIC METHODS ####################
public:
	static Image24_Ptr load_image24(const std::string& filename);
	static Image32_Ptr load_image32(const std::string& filename);
	static Image24_Ptr load_streamed_image24(std::istream& is);
	static Image32_Ptr load_streamed_image32(std::istream& is);

	//#################### PRIVATE METHODS ####################
private:
	static Image24_Ptr decode_png_24(const std::vector<unsigned char>& buffer, const std::string& filename = "");
	static Image32_Ptr decode_png_32(const std::vector<unsigned char>& buffer, const std::string& filename = "");
};

}

#endif
