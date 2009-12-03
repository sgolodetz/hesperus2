/***
 * hesperus: ImageLoader.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ImageLoader.h"

#include <hesp/exceptions/Exception.h>
#include "BitmapLoader.h"
#include "PNGLoader.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
Image24_Ptr ImageLoader::load_image24(const std::string& filename)
{
	std::string extension = determine_extension(filename);
	if(extension == "bmp") return BitmapLoader::load_image24(filename);
	else if(extension == "png") return PNGLoader::load_image24(filename);
	else throw Exception("Unknown image file extension: " + extension);
}

Image32_Ptr ImageLoader::load_image32(const std::string& filename)
{
	std::string extension = determine_extension(filename);
	if(extension == "bmp") throw Exception("Loading of 32-bit bitmaps is currently not supported");
	else if(extension == "png") return PNGLoader::load_image32(filename);
	else throw Exception("Unknown image file extension: " + extension);
}

//#################### PRIVATE METHODS ####################
std::string ImageLoader::determine_extension(const std::string& filename)
{
	std::string::size_type i = filename.find_last_of('.');
	if(i == std::string::npos) throw Exception("Could not deduce image file type from non-existent file extension");
	return filename.substr(i+1);
}

}
