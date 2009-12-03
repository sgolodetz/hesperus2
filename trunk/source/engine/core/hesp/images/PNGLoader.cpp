/***
 * hesperus: PNGLoader.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "PNGLoader.h"

#include <fstream>

#include <lodepng.h>

#include <hesp/exceptions/FileNotFoundException.h>
#include "PixelTypes.h"
#include "SimpleImage.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
/**
Loads a 24-bit PNG from a file.

@param filename		The name of the file in which the PNG is stored
@return				An Image24_Ptr holding the representation of the image
*/
Image24_Ptr PNGLoader::load_image24(const std::string& filename)
{
	std::vector<unsigned char> buffer;
	LodePNG::loadFile(buffer, filename);
	if(buffer.empty()) throw FileNotFoundException(filename);
	return decode_png_24(buffer, filename);
}

/**
Loads a 32-bit PNG from a file.

@param filename		The name of the file in which the PNG is stored
@return				An Image32_Ptr holding the representation of the image
*/
Image32_Ptr PNGLoader::load_image32(const std::string& filename)
{
	std::vector<unsigned char> buffer;
	LodePNG::loadFile(buffer, filename);
	if(buffer.empty()) throw FileNotFoundException(filename);
	return decode_png_32(buffer, filename);
}

/**
Loads a 24-bit PNG from a std::istream.

@param is	The std::istream from which to load the bitmap
@return		An Image24_Ptr holding the representation of the image
*/
Image24_Ptr PNGLoader::load_streamed_image24(std::istream& is)
{
	// TODO: There may be endian issues with this if we ever port to another platform.
	unsigned long len;
	is.read(reinterpret_cast<char*>(&len), sizeof(unsigned long));
	std::vector<unsigned char> buffer(len);
	is.read(reinterpret_cast<char*>(&buffer[0]), len);
	return decode_png_24(buffer);
}

/**
Loads a 32-bit PNG from a std::istream.

@param is	The std::istream from which to load the bitmap
@return		An Image32_Ptr holding the representation of the image
*/
Image32_Ptr PNGLoader::load_streamed_image32(std::istream& is)
{
	// TODO: There may be endian issues with this if we ever port to another platform.
	unsigned long len;
	is.read(reinterpret_cast<char*>(&len), sizeof(unsigned long));
	std::vector<unsigned char> buffer(len);
	is.read(reinterpret_cast<char*>(&buffer[0]), len);
	return decode_png_32(buffer);
}

//#################### PRIVATE METHODS ####################
/**
Decodes a PNG to a 24-bit image.

@param buffer		A buffer containing the loaded raw image data
@param filename		The name of the file from which the data was originally loaded (if known, else "")
@return				An Image24_Ptr holding the representation of the image
*/
Image24_Ptr PNGLoader::decode_png_24(const std::vector<unsigned char>& buffer, const std::string& filename)
{
	// Decode the PNG.
	std::vector<unsigned char> data;
	LodePNG::Decoder decoder;
	decoder.decode(data, &buffer[0], buffer.size());
	if(decoder.hasError())
	{
		if(filename != "") throw Exception("An error occurred whilst trying to decode the PNG in " + filename);
		else throw Exception("An error occurred whilst trying to decode the PNG");
	}

	// Construct the 24-bit image.
	int width = decoder.getWidth();
	int height = decoder.getHeight();

	const int pixelCount = width*height;
	Pixel24 *const pixels = new Pixel24[pixelCount];

	unsigned char *src = &data[0];
	Pixel24 *dest = pixels;
	for(int i=0; i<pixelCount; ++i)
	{
		*dest = Pixel24(*src, *(src+1), *(src+2));
		++dest;
		src += 4;	// note that we skip the alpha component each time (that's why it's += 4 not += 3)
	}

	return Image24_Ptr(new SimpleImage24(pixels, width, height));
}

/**
Decodes a PNG to a 32-bit image.

@param buffer		A buffer containing the loaded raw image data
@param filename		The name of the file from which the data was originally loaded (if known, else "")
@return				An Image32_Ptr holding the representation of the image
*/
Image32_Ptr PNGLoader::decode_png_32(const std::vector<unsigned char>& buffer, const std::string& filename)
{
	// Decode the PNG.
	std::vector<unsigned char> data;
	LodePNG::Decoder decoder;
	decoder.decode(data, &buffer[0], buffer.size());
	if(decoder.hasError())
	{
		if(filename != "") throw Exception("An error occurred whilst trying to decode the PNG in " + filename);
		else throw Exception("An error occurred whilst trying to decode the PNG");
	}

	// Construct the 32-bit image.
	int width = decoder.getWidth();
	int height = decoder.getHeight();

	const int pixelCount = width*height;
	Pixel32 *const pixels = new Pixel32[pixelCount];

	unsigned char *src = &data[0];
	Pixel32 *dest = pixels;
	for(int i=0; i<pixelCount; ++i)
	{
		*dest = Pixel32(*src, *(src+1), *(src+2), *(src+3));
		++dest;
		src += 4;
	}

	return Image32_Ptr(new SimpleImage32(pixels, width, height));
}

}
