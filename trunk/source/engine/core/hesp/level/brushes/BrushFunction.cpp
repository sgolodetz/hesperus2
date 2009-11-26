/***
 * hesperus: BrushFunction.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "BrushFunction.h"

#include <iostream>
#include <string>

#include <hesp/exceptions/Exception.h>

namespace hesp {

//#################### GLOBAL OPERATORS ####################
std::ostream& operator<<(std::ostream& os, BrushFunction rhs)
{
	switch(rhs)
	{
		case BF_COLLISION:	os << "COLLISION";	break;
		case BF_DETAIL:		os << "DETAIL";		break;
		case BF_HINT:		os << "HINT";		break;
		case BF_NORMAL:		os << "NORMAL";		break;
		case BF_WATER:		os << "WATER";		break;
		default:			throw Exception("Forgot to handle one of the brush function cases");
	}
	return os;
}

std::istream& operator>>(std::istream& is, BrushFunction& rhs)
{
	std::string s;
	is >> s;
	if(s == "COLLISION")	rhs = BF_COLLISION;
	else if(s == "DETAIL")	rhs = BF_DETAIL;
	else if(s == "HINT")	rhs = BF_HINT;
	else if(s == "NORMAL")	rhs = BF_NORMAL;
	else if(s == "WATER")	rhs = BF_WATER;
	else throw Exception("Unexpected brush function " + s);
	return is;
}

}
