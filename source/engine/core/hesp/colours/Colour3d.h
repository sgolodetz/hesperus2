/***
 * hesperus: Colour3d.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_COLOUR3D
#define H_HESP_COLOUR3D

#include <iostream>
#include <string>
#include <vector>

namespace hesp {

struct Colour3d
{
	//#################### PUBLIC VARIABLES ####################
	double r, g, b;

	//#################### CONSTRUCTORS ####################
	Colour3d();
	Colour3d(double r_, double g_, double b_);
	Colour3d(const std::vector<std::string>& components);

	//#################### PUBLIC OPERATORS ####################
	Colour3d& operator+=(const Colour3d& rhs);
	Colour3d& operator*=(double factor);
	Colour3d& operator/=(double factor);
};

//#################### GLOBAL OPERATORS ####################
Colour3d operator*(double factor, const Colour3d& c);
Colour3d operator*(const Colour3d& c, double factor);
std::ostream& operator<<(std::ostream& os, const Colour3d& c);
std::istream& operator>>(std::istream& is, Colour3d& c);

}

#endif
