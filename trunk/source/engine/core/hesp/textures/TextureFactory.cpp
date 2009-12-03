/***
 * hesperus: TextureFactory.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "TextureFactory.h"

#include <hesp/ogl/WrappedGL.h>
#include <GL/glu.h>

#include <hesp/exceptions/InvalidParameterException.h>
#include "Image24Texture.h"
#include "Image32Texture.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
/**
Creates a texture from a 24-bit image.

@param image		The image from which to create the texture
@param clamp		Whether or not we want to clamp the texture at the edges
@return				The created texture
*/
Texture_Ptr TextureFactory::create_texture24(const Image24_CPtr& image, bool clamp)
{
	int width = image->width(), height = image->height();
	check_dimensions(width, height);
	return Texture_Ptr(new Image24Texture(image, clamp));
}

/**
Creates a texture from a 32-bit image.

@param image		The image from which to create the texture
@param clamp		Whether or not we want to clamp the texture at the edges
@return				The created texture
*/
Texture_Ptr TextureFactory::create_texture32(const Image32_CPtr& image, bool clamp)
{
	int width = image->width(), height = image->height();
	check_dimensions(width, height);
	return Texture_Ptr(new Image32Texture(image, clamp));
}

//#################### PRIVATE METHODS ####################
/**
Checks that the proposed dimensions of the texture are valid.

@param width						The proposed width of the texture
@param height						The proposed height of the texture
@throws InvalidParameterException	If the proposed dimensions are invalid
*/
void TextureFactory::check_dimensions(int width, int height)
{
	if(width < 2 || width > 1024 || height < 2 || height > 1024) throw InvalidParameterException("Image dimensions out of range");
	if(!is_power_of_two(width) || !is_power_of_two(height)) throw InvalidParameterException("Image dimensions are not powers of two");
}

/**
Returns whether or not n is a power of two.
*/
bool TextureFactory::is_power_of_two(int n)
{
	if(n < 1) return false;

	while(n != 1)
	{
		if(n % 2 != 0) return false;
		n = n >> 1;
	}

	return true;
}

}
