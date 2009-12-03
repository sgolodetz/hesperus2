/***
 * hesperus: Image24Texture.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Image24Texture.h"

#include <vector>

#include <GL/glu.h>

#include <hesp/images/PixelTypes.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
Image24Texture::Image24Texture(const Image24_CPtr& image, bool clamp)
:	Texture(clamp), m_image(image)
{
	reload();
}

//#################### PROTECTED METHODS ####################
void Image24Texture::reload_image() const
{
	int width = m_image->width();
	int height = m_image->height();
	int size = width*height;
	std::vector<unsigned char> pixels(size*3);
	for(int i=0; i<size; ++i)
	{
		const Image24::Pixel& p = (*m_image)(i);
		pixels[i*3]		= p.r();
		pixels[i*3+1]	= p.g();
		pixels[i*3+2]	= p.b();
	}
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, &pixels[0]);
}

}
