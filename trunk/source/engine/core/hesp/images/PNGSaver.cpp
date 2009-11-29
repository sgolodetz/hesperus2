/***
 * hesperus: PNGSaver.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "PNGSaver.h"

#include <lodepng.h>

#include "PixelTypes.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
/**
Saves a 24-bit PNG to a file.

@param filename		The name of the file to which to save the image
@param image		An Image24_CPtr holding the representation of the image
*/
void PNGSaver::save_image24(const std::string& filename, const Image24_CPtr& image)
{
	std::vector<unsigned char> buffer;
	encode_png(image, buffer);
	LodePNG::saveFile(buffer, filename);
}

/**
Saves a 24-bit PNG to a std::ostream.

@param os		The std::ostream to which to save the image
@param image	An Image24_CPtr holding the representation of the image
*/
void PNGSaver::save_streamed_image24(std::ostream& os, const Image24_CPtr& image)
{
	std::vector<unsigned char> buffer;
	encode_png(image, buffer);

	unsigned long len = static_cast<unsigned long>(buffer.size());

	// TODO: There may be endian issues with this if we ever port to another platform.
	os.write(reinterpret_cast<char*>(&len), sizeof(unsigned long));
	os.write(reinterpret_cast<char*>(&buffer[0]), len);
}

//#################### PRIVATE METHODS ####################
/**
Encodes a 24-bit image into PNG format.

@param image	The image
@param buffer	The buffer into which to store the result
*/
void PNGSaver::encode_png(const Image24_CPtr& image, std::vector<unsigned char>& buffer)
{
	int width = image->width();
	int height = image->height();
	const int pixelCount = width*height;

	// Construct the image data array.
	std::vector<unsigned char> data(pixelCount*4);
	unsigned char *p = &data[0];
	for(int y=0; y<height; ++y)
		for(int x=0; x<width; ++x)
		{
			Pixel24 pixel = (*image)(x,y);
			*p		= pixel.r();
			*(p+1)	= pixel.g();
			*(p+2)	= pixel.b();
			*(p+3)	= 255;
			p += 4;
		}

	// Encode the PNG.
	LodePNG::encode(buffer, &data[0], width, height);
}

}
