/***
 * hesperus: BrushFunction.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_BRUSHFUNCTION
#define H_HESP_BRUSHFUNCTION

#include <iosfwd>

namespace hesp {

//#################### ENUMERATIONS ####################
enum BrushFunction
{
	BF_COLLISION,
	BF_DETAIL,
	BF_HINT,
	BF_NORMAL,
	BF_WATER,
};

//#################### GLOBAL OPERATORS ####################
std::ostream& operator<<(std::ostream& os, BrushFunction rhs);
std::istream& operator>>(std::istream& is, BrushFunction& rhs);

}

#endif
