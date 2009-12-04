/***
 * hesperus: TextRenderer.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "TextRenderer.h"

#ifdef _WIN32
	#include <windowsx.h>
#endif

#include <hesp/exceptions/Exception.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
TextRenderer::TextRenderer(const std::string& typeface, int pointSize)
{
#ifdef _WIN32
	m_base = glGenLists(128);

	HDC hdc = wglGetCurrentDC();
	m_pixelHeight = pointSize * GetDeviceCaps(hdc, LOGPIXELSY) / 72;
	HFONT font = CreateFont(-m_pixelHeight, 0, 0, 0, FW_NORMAL, false, false, false, ANSI_CHARSET, OUT_TT_PRECIS,
							CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH|FF_DONTCARE, typeface.c_str());
	HFONT oldFont = (HFONT)SelectObject(hdc, font);

	// Try and generate the font bitmaps - note that this sometimes has to be called multiple times
	// to get it to work, which is seriously dodgy. If we fail several times in a row, just give up.
	bool succeeded = false;
	const int MAX_TRIES = 3;
	for(int i=0; i<MAX_TRIES && !succeeded; ++i)
	{
		succeeded = wglUseFontBitmaps(hdc, 32, 96, m_base+32) != 0;
	}

	SelectObject(hdc, oldFont);
	DeleteFont(font);
	if(!succeeded) throw Exception("The text renderer could not be constructed");
#else
	// FIXME: Implement text rendering on other platforms.
	throw Exception("Text rendering not yet implemented on non-Windows platforms.");
#endif
}

//#################### DESTRUCTOR ####################
TextRenderer::~TextRenderer()
{
	glDeleteLists(m_base, 128);
}

//#################### PUBLIC METHODS ####################
TextRenderer::TextExtents TextRenderer::text_extents(const std::string& text) const
{
#ifdef _WIN32
	// Get the dimensions of the text to be rendered.
	HDC hdc = wglGetCurrentDC();
	SIZE dimensions;
	GetTextExtentPoint32(hdc, text.c_str(), static_cast<int>(text.length()), &dimensions);
	return TextExtents(dimensions.cx, dimensions.cy);
#else
	// FIXME: Implement text rendering on other platforms.
	throw Exception("Text rendering not yet implemented on non-Windows platforms.");
#endif
}

int TextRenderer::text_height_in_pixels() const
{
	return m_pixelHeight;
}

void TextRenderer::write(const std::string& text, double x, double y, const Colour3d& colour) const
{
	glPushAttrib(GL_LIST_BIT);
		glColor3d(colour.r, colour.g, colour.b);
		glRasterPos2d(x, y + text_height_in_pixels());
		glListBase(m_base);
		glCallLists(static_cast<GLsizei>(text.length()), GL_UNSIGNED_BYTE, text.c_str());
	glPopAttrib();
}

void TextRenderer::write_aligned(const std::string& text, double x, double y, HAlignFlag hflag, VAlignFlag vflag,
								 const Colour3d& colour) const
{
	if(hflag == HALIGN_LEFT && vflag == VALIGN_TOP)
	{
		write(text, x, y, colour);
	}
	else
	{
		TextExtents extents = text_extents(text);

		double newX = x, newY = y;
		if(hflag & HALIGN_RIGHT) newX -= extents.width;
		else if(hflag & HALIGN_CENTRE) newX -= extents.width / 2.0;
		if(vflag & VALIGN_BOTTOM) newY -= extents.height;
		else if(vflag & VALIGN_CENTRE) newY -= extents.height / 2.0;

		write(text, newX, newY, colour);
	}
}

void TextRenderer::write_centred(const std::string& text, double x, double y, const Colour3d& colour) const
{
	write_aligned(text, x, y, HALIGN_CENTRE, VALIGN_CENTRE, colour);
}

}
