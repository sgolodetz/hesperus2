/***
 * hesperus: Extents.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_EXTENTS
#define H_HESP_EXTENTS

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

/**
This class represents a rectangle bounding the space occupied by
a component on the screen.
*/
struct Extents
{
	//#################### PRIVATE VARIABLES ####################
private:
	int m_left, m_top, m_right, m_bottom;

	//#################### CONSTRUCTORS ####################
public:
	Extents(int left, int top, int right, int bottom)
	:	m_left(left), m_top(top), m_right(right), m_bottom(bottom)
	{}

	//#################### PUBLIC METHODS ####################
public:
	int bottom() const	{ return m_bottom; }

	bool contains(int x, int y) const
	{
		return m_left <= x && x <= m_right && m_top <= y && y <= m_bottom;
	}

	int height() const	{ return m_bottom - m_top; }
	int left() const	{ return m_left; }
	int right() const	{ return m_right; }
	int top() const		{ return m_top; }

	Extents translate(int dx, int dy) const
	{
		return Extents(m_left+dx, m_top+dy, m_right+dx, m_bottom+dy);
	}

	int width() const	{ return m_right - m_left; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Extents> Extents_Ptr;

}

#endif
