/***
 * hesperus: Screen.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "Screen.h"

#include <SDL.h>

#include <hesp/ogl/WrappedGL.h>
#include <GL/glu.h>

namespace hesp {

//#################### SINGLETON IMPLEMENTATION ####################
Screen::Screen() {}

Screen& Screen::instance()
{
	static Screen s_instance;
	return s_instance;
}

//#################### PUBLIC METHODS ####################
const Extents& Screen::dimensions() const
{
	return *m_dimensions;
}

void Screen::fit()
{
	GUIContainer::fit(*m_dimensions, NULL);
}

void Screen::render() const
{
	const int& x1 = m_extents->left(), y1 = m_extents->top(), x2 = m_extents->right(), y2 = m_extents->bottom();
	glViewport(x1, y1, x2-x1, y2-y1);

	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GUIContainer::render();

	SDL_GL_SwapBuffers();
}

void Screen::set_dimensions(const Extents& dimensions)
{
	m_dimensions.reset(new Extents(dimensions));
}

void Screen::set_ortho_viewport(const Extents& extents)
{
	int screenHeight = m_extents->bottom() - m_extents->top();

	const int& x1 = extents.left(), y1 = extents.top(), x2 = extents.right(), y2 = extents.bottom();
	glViewport(x1, screenHeight - y2, x2-x1, y2-y1);

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluOrtho2D(-1, x2+1-x1, y2+1-y1, -1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Screen::set_persp_viewport(const Extents& extents, double fovY, double zNear, double zFar)
{
	int screenHeight = m_extents->bottom() - m_extents->top();

	const int& x1 = extents.left(), y1 = extents.top(), x2 = extents.right(), y2 = extents.bottom();
	glViewport(x1, screenHeight - y2, x2-x1, y2-y1);

	double width = x2 - x1, height = y2 - y1;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovY, width / height, zNear, zFar);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

}
