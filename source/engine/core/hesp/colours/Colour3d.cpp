/***
 * hesperus: Colour3d.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "Colour3d.h"

#include <boost/lexical_cast.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <hesp/exceptions/Exception.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
Colour3d::Colour3d()
:	r(0), g(0), b(0)
{}

Colour3d::Colour3d(double r_, double g_, double b_)
:	r(r_), g(g_), b(b_)
{}

Colour3d::Colour3d(const std::vector<std::string>& components)
{
	if(components.size() != 3) throw Exception("Incorrect number of colour components");

	try
	{
		r = lexical_cast<double>(components[0]);
		g = lexical_cast<double>(components[1]);
		b = lexical_cast<double>(components[2]);
	}
	catch(bad_lexical_cast&) { throw Exception("One of the colour components is not a number"); }
}

//#################### PUBLIC OPERATORS ####################
Colour3d& Colour3d::operator+=(const Colour3d& rhs)
{
	r += rhs.r;
	g += rhs.g;
	b += rhs.b;
	return *this;
}

Colour3d& Colour3d::operator*=(double factor)
{
	r *= factor;
	g *= factor;
	b *= factor;
	return *this;
}

Colour3d& Colour3d::operator/=(double factor)
{
	*this *= 1/factor;
	return *this;
}

//#################### GLOBAL OPERATORS ####################
Colour3d operator*(double factor, const Colour3d& c)
{
	return Colour3d(factor*c.r, factor*c.g, factor*c.b);
}

Colour3d operator*(const Colour3d& c, double factor)
{
	return Colour3d(factor*c.r, factor*c.g, factor*c.b);
}

std::ostream& operator<<(std::ostream& os, const Colour3d& c)
{
	os << "[ " << c.r << ' ' << c.g << ' ' << c.b << " ]";
	return os;
}

std::istream& operator>>(std::istream& is, Colour3d& c)
{
	std::ios_base::fmtflags oldFlags = is.flags();
	is.setf(std::ios_base::skipws);

	std::string dummy;
	is >> dummy >> c.r >> c.g >> c.b >> dummy;

	is.flags(oldFlags);

	return is;
}

}
