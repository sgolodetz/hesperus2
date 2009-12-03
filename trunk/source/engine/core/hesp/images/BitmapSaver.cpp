/***
 * hesperus: BitmapSaver.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "BitmapSaver.h"

#include <fstream>
#include <vector>

#ifdef _WIN32
	// Provides BITMAPFILEHEADER, BITMAPINFOHEADER, etc.
	#include <windows.h>
#else
	#error Need to define the relevant bitmap structs for non-Windows platforms.
#endif

#include <hesp/exceptions/Exception.h>
#include <hesp/images/SimpleImage.h>
#include "PixelTypes.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
/**
Saves a 24-bit bitmap to a file.

@param filename		The name of the file to which to save the image
@param image		An Image24_CPtr holding the representation of the image
*/
void BitmapSaver::save_image24(const std::string& filename, const Image24_CPtr& image)
{
	std::ofstream fs(filename.c_str(), std::ios_base::binary);
	if(fs.fail()) throw Exception("Could not open " + filename + " for writing");
	else save_streamed_image24(fs, image);
}

/**
Saves a 24-bit bitmap to a std::ostream.

@param os		The std::ostream to which to save the image
@param image	An Image24_CPtr holding the representation of the image
*/
void BitmapSaver::save_streamed_image24(std::ostream& os, const Image24_CPtr& image)
{
	const int w = image->width();
	const int h = image->height();

	// Prepare the headers.
	BITMAPINFOHEADER bih = {0};		// initialise all fields to 0
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biWidth = w;
	bih.biHeight = h;
	bih.biPlanes = 1;
	bih.biBitCount = 24;
	bih.biCompression = BI_RGB;
	bih.biSizeImage = bih.biWidth * bih.biHeight * (bih.biBitCount / 8);

	BITMAPFILEHEADER bfh = {0};
	bfh.bfType = 0x4D42;
	bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bfh.bfSize = bfh.bfOffBits + bih.biSizeImage;

	// Prepare the data.
	std::vector<unsigned char> data;
	data.reserve(bih.biSizeImage);
	for(int y=h-1; y>=0; --y)
	{
		for(int x=0; x<w; ++x)
		{
			Pixel24 p = (*image)(x,y);
			data.push_back(p.b());
			data.push_back(p.g());
			data.push_back(p.r());
		}
	}

	// Write the headers.
	os.write(reinterpret_cast<char*>(&bfh), sizeof(BITMAPFILEHEADER));
	os.write(reinterpret_cast<char*>(&bih), sizeof(BITMAPINFOHEADER));

	// Write the image data.
	os.write(reinterpret_cast<char *>(&data[0]), bih.biSizeImage);
}

}
