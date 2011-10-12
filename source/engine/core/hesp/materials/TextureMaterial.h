/***
 * hesperus: TextureMaterial.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_TEXTUREMATERIAL
#define H_HESP_TEXTUREMATERIAL

#include "Material.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class Texture> Texture_Ptr;

class TextureMaterial : public Material
{
	//#################### PRIVATE VARIABLES ####################
private:
	Texture_Ptr m_texture;

	//#################### CONSTRUCTORS ####################
public:
	explicit TextureMaterial(const Texture_Ptr& texture);

	//#################### PUBLIC METHODS ####################
public:
	void apply() const;
	bool uses_texcoords() const;
};

}

#endif
