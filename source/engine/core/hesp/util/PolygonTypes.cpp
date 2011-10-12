/***
 * hesperus: PolygonTypes.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "PolygonTypes.h"

#include <iostream>

namespace hesp {

//#################### GLOBAL OPERATORS ####################
std::ostream& operator<<(std::ostream& os, const ColPolyAuxData& rhs)
{
	os << "[ " << rhs.m_mapIndex << ' ' << rhs.m_walkable << " ]";
	return os;
}

std::istream& operator>>(std::istream& is, ColPolyAuxData& rhs)
{
	std::string bracket;
	is >> std::skipws;
	is >> bracket >> rhs.m_mapIndex >> rhs.m_walkable >> bracket;
	is >> std::noskipws;
	return is;
}

}
