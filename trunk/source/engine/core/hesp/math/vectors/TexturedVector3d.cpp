/***
 * hesperus: TexturedVector3d.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "TexturedVector3d.h"

#include <boost/lexical_cast.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <hesp/exceptions/Exception.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
TexturedVector3d::TexturedVector3d() : x(0), y(0), z(0), u(0), v(0) {}

TexturedVector3d::TexturedVector3d(double x_, double y_, double z_, double u_, double v_)
:	x(x_), y(y_), z(z_), u(u_), v(v_)
{}

TexturedVector3d::TexturedVector3d(const std::vector<std::string>& components)
{
	if(components.size() != 5) throw Exception("Incorrect number of vector components");

	try
	{
		x = lexical_cast<double>(components[0]);
		y = lexical_cast<double>(components[1]);
		z = lexical_cast<double>(components[2]);
		u = lexical_cast<double>(components[3]);
		v = lexical_cast<double>(components[4]);
	}
	catch(bad_lexical_cast&) { throw Exception("One of the vector components is not a number"); }
}

//#################### PUBLIC OPERATORS ####################
TexturedVector3d& TexturedVector3d::operator+=(const TexturedVector3d& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	u += rhs.u;
	v += rhs.v;
	return *this;
}

TexturedVector3d& TexturedVector3d::operator-=(const TexturedVector3d& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	u -= rhs.u;
	v -= rhs.v;
	return *this;
}

TexturedVector3d& TexturedVector3d::operator*=(double factor)
{
	x *= factor;
	y *= factor;
	z *= factor;
	u *= factor;
	v *= factor;
	return *this;
}

TexturedVector3d::operator Vector3d() const
{
	return Vector3d(x,y,z);
}

//#################### GLOBAL OPERATORS ####################
TexturedVector3d operator+(const TexturedVector3d& lhs, const TexturedVector3d& rhs)
{
	TexturedVector3d copy(lhs);
	copy += rhs;
	return copy;
}

TexturedVector3d operator-(const TexturedVector3d& lhs, const TexturedVector3d& rhs)
{
	TexturedVector3d copy(lhs);
	copy -= rhs;
	return copy;
}

TexturedVector3d operator*(double factor, const TexturedVector3d& v)
{
	TexturedVector3d copy(v);
	copy *= factor;
	return copy;
}

std::ostream& operator<<(std::ostream& os, const TexturedVector3d& v)
{
	os << "( " << v.x << ' ' << v.y << ' ' << v.z << ' ' << v.u << ' ' << v.v << " )";
	return os;
}

}
