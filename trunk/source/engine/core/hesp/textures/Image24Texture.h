/***
 * hesperus: Image24Texture.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_IMAGE24TEXTURE
#define H_HESP_IMAGE24TEXTURE

#include <hesp/images/Image.h>
#include "Texture.h"

namespace hesp {

/**
This class represents OpenGL textures which are created from 24-bit images.
The image is stored within the texture to facilitate easy recreation if the
texture gets destroyed (e.g. due to screen resolution changes). This is not
a particularly good use of memory (it doubles memory requirements for the
textures), but it makes life easier.
*/
class Image24Texture : public Texture
{
	//#################### FRIENDS ####################
	friend class TextureFactory;

	//#################### PRIVATE VARIABLES ####################
private:
	Image24_CPtr m_image;

	//#################### CONSTRUCTORS ####################
protected:
	Image24Texture(const Image24_CPtr& image, bool clamp);

	//#################### PROTECTED METHODS ####################
protected:
	void reload_image() const;
};

}

#endif
