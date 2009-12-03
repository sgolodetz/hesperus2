/***
 * hesperus: Texture.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "Texture.h"

#define GL_CLAMP_TO_EDGE 0x812F		// this wrapping mode is only defined in OpenGL 1.2, so it's not in the normal header (unfortunately)

namespace hesp {

//#################### HELPER CLASSES ####################
struct TextureDeleter
{
	void operator()(void *p)
	{
		GLuint *id = static_cast<GLuint*>(p);
		if(glIsTexture(*id)) glDeleteTextures(1, id);
		delete p;
	}
};

//#################### CONSTRUCTORS ####################
/**
Constructs an empty texture (the subclass constructor will initialise it).

@param clamp	Whether or not the texture should be clamped to its edges (rather than wrapped)
*/
Texture::Texture(bool clamp)
:	m_clamp(clamp)
{}

//#################### DESTRUCTOR ####################
Texture::~Texture() {}

//#################### PUBLIC METHODS ####################
/**
Binds the texture to GL_TEXTURE_2D (reloads it first if necessary).
*/
void Texture::bind() const
{
	if(!glIsTexture(*m_id)) reload();
	glBindTexture(GL_TEXTURE_2D, *m_id);
}

//#################### PROTECTED METHODS ####################
/**
Reloads the texture.
*/
void Texture::reload() const
{
	GLuint id;
	glGenTextures(1, &id);
	set_id(id);

	glBindTexture(GL_TEXTURE_2D, id);

	// Enable trilinear filtering for this texture when minifying.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// Clamp the texture if necessary (useful for things like lightmaps, for example).
	if(m_clamp)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	// Reload the actual image.
	reload_image();
}

/**
Sets the texture ID (for use by subclasses during reloading).

@param id	The new texture ID
*/
void Texture::set_id(GLuint id) const
{
	m_id.reset(new GLuint(id), TextureDeleter());
}

}
