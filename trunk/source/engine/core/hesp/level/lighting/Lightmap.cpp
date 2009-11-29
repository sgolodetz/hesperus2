/***
 * hesperus: Lightmap.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "Lightmap.h"

#include <hesp/exceptions/InvalidParameterException.h>
#include <hesp/images/PixelTypes.h>
#include <hesp/images/SimpleImage.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
Lightmap::Lightmap(int rows, int cols, const Colour3d& initial)
:	m_lumels(rows), m_rows(rows), m_cols(cols)
{
	for(int r=0; r<rows; ++r)
	{
		m_lumels[r].resize(cols, initial);
	}
}

//#################### PUBLIC OPERATORS ####################
Colour3d& Lightmap::operator()(int row, int col)
{
	return m_lumels[row][col];
}

const Colour3d& Lightmap::operator()(int row, int col) const
{
	return m_lumels[row][col];
}

Lightmap& Lightmap::operator+=(const Lightmap& rhs)
{
	if(m_rows != rhs.m_rows || m_cols != rhs.m_cols)
		throw InvalidParameterException("Lightmaps are not of the same dimensions");

	for(int r=0; r<m_rows; ++r)
	{
		for(int c=0; c<m_cols; ++c)
		{
			m_lumels[r][c] += rhs.m_lumels[r][c];
		}
	}

	return *this;
}

//#################### PUBLIC METHODS ####################
Image24_Ptr Lightmap::to_image() const
{
	Image24_Ptr image(new SimpleImage24(m_cols, m_rows));

	for(int y=0; y<m_rows; ++y)
	{
		for(int x=0; x<m_cols; ++x)
		{
			double rd = m_lumels[y][x].r * 255;
			double gd = m_lumels[y][x].g * 255;
			double bd = m_lumels[y][x].b * 255;
			if(rd < 0) rd = 0;	else if(rd > 255) rd = 255;
			if(gd < 0) gd = 0;	else if(gd > 255) gd = 255;
			if(bd < 0) bd = 0;	else if(bd > 255) bd = 255;

			unsigned char r = static_cast<unsigned char>(rd);
			unsigned char g = static_cast<unsigned char>(gd);
			unsigned char b = static_cast<unsigned char>(bd);

			image->set(x, y, Pixel24(r,g,b));
		}
	}

	return image;
}

}
