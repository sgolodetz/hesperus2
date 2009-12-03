/***
 * hesperus: Image32Texture.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Image32Texture.h"

#include <vector>

#include <GL/glu.h>

#include <hesp/images/PixelTypes.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
Image32Texture::Image32Texture(const Image32_CPtr& image, bool clamp)
:	Texture(clamp), m_image(image)
{
	reload();
}

//#################### PROTECTED METHODS ####################
void Image32Texture::reload_image() const
{
	int width = m_image->width();
	int height = m_image->height();
	int size = width*height;
	std::vector<unsigned char> pixels(size*4);
	for(int i=0; i<size; ++i)
	{
		const Image32::Pixel& p = (*m_image)(i);
		pixels[i*4]		= p.r();
		pixels[i*4+1]	= p.g();
		pixels[i*4+2]	= p.b();
		pixels[i*4+3]	= p.a();
	}
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]);
}

}
