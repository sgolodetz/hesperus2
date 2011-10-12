/***
 * hesperus: Picture.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "Picture.h"

#include <hesp/ogl/WrappedGL.h>
#include <GL/glu.h>

#include <hesp/images/ImageLoader.h>
#include <hesp/textures/Texture.h>
#include <hesp/textures/TextureFactory.h>
#include "Screen.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
Picture::Picture(const std::string& filename)
{
	m_texture = TextureFactory::create_texture24(ImageLoader::load_image24(filename), true);
}

//#################### PUBLIC METHODS ####################
void Picture::render() const
{
	Screen::instance().set_ortho_viewport(*m_extents);

	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);

	int width = m_extents->width(), height = m_extents->height();
	m_texture->bind();
	glBegin(GL_QUADS);
		glColor3d(1,1,1);
		glTexCoord2d(0,0);	glVertex2d(0,0);
		glTexCoord2d(1,0);	glVertex2d(width,0);
		glTexCoord2d(1,1);	glVertex2d(width,height);
		glTexCoord2d(0,1);	glVertex2d(0,height);
	glEnd();

	glPopAttrib();
}

}
