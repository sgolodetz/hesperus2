/***
 * hesperus: TextureMaterial.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "TextureMaterial.h"

#include <hesp/textures/Texture.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
TextureMaterial::TextureMaterial(const Texture_Ptr& texture)
:	m_texture(texture)
{}

//#################### PUBLIC METHODS ####################
void TextureMaterial::apply() const
{
	glEnable(GL_TEXTURE_2D);
	m_texture->bind();
	glColor3d(1,1,1);
}

bool TextureMaterial::uses_texcoords() const
{
	return true;
}

}
