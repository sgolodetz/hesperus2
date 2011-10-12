/***
 * hesperus: Button.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Button.h"

#include <hesp/images/ImageLoader.h>
#include <hesp/input/InputState.h>
#include <hesp/textures/Texture.h>
#include <hesp/textures/TextureFactory.h>
#include "Screen.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
Button::Button(const std::string& inactiveFilename, const std::string& activeFilename)
{
	construct(inactiveFilename, activeFilename, boost::none, boost::none);
}

Button::Button(const std::string& inactiveFilename, const std::string& activeFilename,
			   const Handler& mousePressedHandler)
{
	construct(inactiveFilename, activeFilename, mousePressedHandler, boost::none);
}

Button::Button(const std::string& inactiveFilename, const std::string& activeFilename,
			   const boost::none_t&, const Handler& mouseReleasedHandler)
{
	construct(inactiveFilename, activeFilename, boost::none, mouseReleasedHandler);
}

Button::Button(const std::string& inactiveFilename, const std::string& activeFilename,
			   const Handler& mousePressedHandler, const Handler& mouseReleasedHandler)
{
	construct(inactiveFilename, activeFilename, mousePressedHandler, mouseReleasedHandler);
}

//#################### PUBLIC METHODS ####################
void Button::handle_input(InputState& input)
{
	switch(m_state)
	{
	case HOVERED:
		if(input.mouse_button_down(MOUSE_BUTTON_LEFT) &&
		   m_extents->contains(input.mouse_pressed_x(MOUSE_BUTTON_LEFT), input.mouse_pressed_y(MOUSE_BUTTON_LEFT)))
		{
			m_state = PRESSED;
			if(m_mousePressedHandler) (*m_mousePressedHandler)();
		}
		else if(input.mouse_position_known() && !m_extents->contains(input.mouse_position_x(), input.mouse_position_y()))
		{
			m_state = RELEASED;
		}
		break;

	case RELEASED:
		if(input.mouse_position_known() && m_extents->contains(input.mouse_position_x(), input.mouse_position_y()))
		{
			m_state = HOVERED;
		}
		break;

	case PRESSED:
		if(!input.mouse_button_down(MOUSE_BUTTON_LEFT))
		{
			m_state = RELEASED;
			if(m_mouseReleasedHandler && m_extents->contains(input.mouse_position_x(), input.mouse_position_y()))
			{
				(*m_mouseReleasedHandler)();
			}
		}
		else if(input.mouse_position_known() && !m_extents->contains(input.mouse_position_x(), input.mouse_position_y()))
		{
			m_state = PRESSED_MOVED;
		}
		break;

	case PRESSED_MOVED:
		if(!input.mouse_button_down(MOUSE_BUTTON_LEFT))
		{
			m_state = RELEASED;
		}
		else if(input.mouse_position_known() && m_extents->contains(input.mouse_position_x(), input.mouse_position_y()))
		{
			m_state = PRESSED;
		}
		break;
	}
}

void Button::render() const
{
	Screen::instance().set_ortho_viewport(*m_extents);

	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);

	int width = m_extents->width(), height = m_extents->height();

	if(m_state == HOVERED || m_state == PRESSED)	m_activeTexture->bind();
	else											m_inactiveTexture->bind();

	// FIXME: This is very similar to the code in Picture::render() - there's potential for refactoring here.
	glBegin(GL_QUADS);
		glColor3d(1,1,1);
		glTexCoord2d(0,0);	glVertex2d(0,0);
		glTexCoord2d(1,0);	glVertex2d(width,0);
		glTexCoord2d(1,1);	glVertex2d(width,height);
		glTexCoord2d(0,1);	glVertex2d(0,height);
	glEnd();

	glPopAttrib();
}

//#################### PRIVATE METHODS ####################
void Button::construct(const std::string& inactiveFilename, const std::string& activeFilename,
					   const boost::optional<Handler>& mousePressedHandler,
					   const boost::optional<Handler>& mouseReleasedHandler)
{
	m_inactiveTexture = TextureFactory::create_texture24(ImageLoader::load_image24(inactiveFilename));
	m_activeTexture = TextureFactory::create_texture24(ImageLoader::load_image24(activeFilename));

	m_state = RELEASED;
	m_mousePressedHandler = mousePressedHandler;
	m_mouseReleasedHandler = mouseReleasedHandler;
}

}
