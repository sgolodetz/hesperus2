/***
 * hesperus: TextureFactory.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_TEXTUREFACTORY
#define H_HESP_TEXTUREFACTORY

#include <hesp/images/Image.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class Texture> Texture_Ptr;

class TextureFactory
{
	//#################### PUBLIC METHODS ####################
public:
	static Texture_Ptr create_texture24(const Image24_CPtr& image, bool clamp = false);
	static Texture_Ptr create_texture32(const Image32_CPtr& image, bool clamp = false);

	//#################### PRIVATE METHODS ####################
private:
	static void check_dimensions(int width, int height);
	static bool is_power_of_two(int n);
};

}

#endif
