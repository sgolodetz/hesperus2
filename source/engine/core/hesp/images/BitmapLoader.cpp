/***
 * hesperus: BitmapLoader.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "BitmapLoader.h"

#include <fstream>
#include <vector>

#ifdef _WIN32
	// Provides BITMAPFILEHEADER, BITMAPINFOHEADER, etc.
	#include <windows.h>
#endif

#include <hesp/exceptions/FileNotFoundException.h>
#include <hesp/images/SimpleImage.h>
#include "PixelTypes.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
/**
Loads a 24-bit bitmap from a file.

@param filename		The name of the file in which the bitmap is stored
@return				An Image24_Ptr holding the representation of the image
*/
Image24_Ptr BitmapLoader::load_image24(const std::string& filename)
{
	std::ifstream fs(filename.c_str(), std::ios_base::binary);
	if(fs.fail()) throw FileNotFoundException(filename);
	else return load_streamed_image24(fs);
}

/**
Loads a 24-bit bitmap from a std::istream.

@param is	The std::istream from which to load the bitmap
@return		An Image24_Ptr holding the representation of the image
*/
Image24_Ptr BitmapLoader::load_streamed_image24(std::istream& is)
{
#ifdef _WIN32
	// Read in the file header and check that we're dealing with a valid bitmap.
	BITMAPFILEHEADER bfh;
	is.read(reinterpret_cast<char*>(&bfh), sizeof(BITMAPFILEHEADER));
	if(bfh.bfType != 0x4D42) throw Exception("Error: Not a valid bitmap.");

	// Read in the info header and check that the bit depth is 24.
	BITMAPINFOHEADER bih;
	is.read(reinterpret_cast<char*>(&bih), sizeof(BITMAPINFOHEADER));
	if(bih.biBitCount != 24) throw Exception("Error: This function only loads 24-bit bitmaps.");

	// Read in the actual image data.
	const int dataSize = bfh.bfSize - bfh.bfOffBits;
	std::vector<unsigned char> data(dataSize);
	is.read(reinterpret_cast<char*>(&data[0]), dataSize);

	const int width = bih.biWidth, height = abs(bih.biHeight);

	const bool flipped = bih.biHeight > 0;		// the sign of biHeight indicates whether or not the rows of the image are reversed
	const int byteWidth = width * 3;
	int padWidth = byteWidth;
	while(padWidth % 4 != 0) ++padWidth;
	const int offset = padWidth - byteWidth;	// the amount of padding at the end of each line to ensure that lines are aligned on 4-byte boundaries

	const int pixelCount = width*height;
	Pixel24 *pixels = new Pixel24[pixelCount];

	unsigned char *p = &data[0];
	if(flipped)
	{
		for(int i=height-1; i>=0; --i)
		{
			const int iw = i*width;
			for(int j=0; j<width; ++j)
			{
				pixels[iw+j] = Pixel24(*(p+2), *(p+1), *p);		// note that bitmaps are stored in BGR rather than RGB format
				p += 3;
			}
			p += offset;
		}
	}
	else
	{
		for(int i=0; i<height; ++i)
		{
			const int iw = i*width;
			for(int j=0; j<width; ++j)
			{
				pixels[iw+j] = Pixel24(*(p+2), *(p+1), *p);		// note that bitmaps are stored in BGR rather than RGB format
				p += 3;
			}
		}
	}

	return Image24_Ptr(new SimpleImage24(pixels, width, height));
#else
	// FIXME: Implement bitmap loading on other platforms.
	throw Exception("Bitmap loading not yet implemented on non-Windows platforms");
#endif
}

}
