/***
 * TexturedLitVector3d.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "TexturedLitVector3d.h"

#include <ostream>

#include <boost/lexical_cast.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <hesp/exceptions/Exception.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
TexturedLitVector3d::TexturedLitVector3d(double x_, double y_, double z_, double u_, double v_, double lu_, double lv_)
:	x(x_), y(y_), z(z_), u(u_), v(v_), lu(lu_), lv(lv_)
{}

TexturedLitVector3d::TexturedLitVector3d(const std::vector<std::string>& components)
{
	if(components.size() != 7) throw Exception("Incorrect number of vector components");

	try
	{
		x = lexical_cast<double,std::string>(components[0]);
		y = lexical_cast<double,std::string>(components[1]);
		z = lexical_cast<double,std::string>(components[2]);
		u = lexical_cast<double,std::string>(components[3]);
		v = lexical_cast<double,std::string>(components[4]);
		lu = lexical_cast<double,std::string>(components[5]);
		lv = lexical_cast<double,std::string>(components[6]);
	}
	catch(bad_lexical_cast&) { throw Exception("One of the vector components is not a number"); }
}

//#################### PUBLIC OPERATORS ####################
TexturedLitVector3d::operator Vector3d() const
{
	return Vector3d(x,y,z);
}

//#################### GLOBAL OPERATORS ####################
std::ostream& operator<<(std::ostream& os, const TexturedLitVector3d& v)
{
	os << "( " << v.x << ' ' << v.y << ' ' << v.z << ' ' << v.u << ' ' << v.v << ' ' << v.lu << ' ' << v.lv <<  " )";
	return os;
}

}
