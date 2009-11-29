/***
 * hesperus: SimpleImage.tpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

namespace hesp {

//#################### CONSTRUCTORS ####################
/**
Constructs a blank SimpleImage with the specified width and height.

@param width	The width of the image to construct
@param height	The height of the image to construct
*/
template <typename Pixel>
SimpleImage<Pixel>::SimpleImage(int width, int height)
:	Image(width, height)
{
	int size = width * height;
	Pixel *pixels = new Pixel[size]();	// <-- Note that this *default constructs* every Pixel in the array.
	m_pixels.reset(pixels);
}

/**
Constructs a SimpleImage from the specified pixel data, which should be interpreted as being an image
of size width * height, laid out in rows in memory.

@param pixels	A pointer to the start of the pixel data
@param width	The width of the source image
@param height	The height of the source image
*/
template <typename Pixel>
SimpleImage<Pixel>::SimpleImage(Pixel *pixels, int width, int height)
:	Image(width, height), m_pixels(pixels)
{}

/**
Constructs a SimpleImage from the specified pixel data, which should be interpreted as being an image
of size width * height, laid out in rows in memory.

@param pixels	The pixel data as a shared_array
@param width	The width of the source image
@param height	The height of the source image
*/
template <typename Pixel>
SimpleImage<Pixel>::SimpleImage(const shared_array<Pixel>& pixels, int width, int height)
:	Image(width, height), m_pixels(pixels)
{}

/**
Constructs a SimpleImage from another image with the same pixel type (note that this is effectively
a more useful version of what would be the copy constructor).

@param rhs	The image to copy
*/
template <typename Pixel>
SimpleImage<Pixel>::SimpleImage(const Image_CPtr& rhs)
:	Image(rhs->width(), rhs->height())
{
	int size = m_width * m_height;
	Pixel *pixels = new Pixel[size];
	for(int i=0; i<size; ++i) pixels[i] = rhs(i);
	m_pixels.reset(pixels);
}

//#################### PUBLIC OPERATORS ####################
template <typename Pixel>
const Pixel SimpleImage<Pixel>::operator()(int n) const
{
	return m_pixels[n];
}

template <typename Pixel>
const Pixel SimpleImage<Pixel>::operator()(int x, int y) const
{
	return m_pixels[y*m_width+x];
}

//#################### PUBLIC METHODS ####################
template <typename Pixel>
void SimpleImage<Pixel>::set(int n, const Pixel& pixel)
{
	m_pixels[n] = pixel;
}

template <typename Pixel>
void SimpleImage<Pixel>::set(int x, int y, const Pixel& pixel)
{
	m_pixels[y*m_width+x] = pixel;
}

}
