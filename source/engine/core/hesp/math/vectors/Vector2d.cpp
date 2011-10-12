/***
 * hesperus: Vector2d.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "Vector2d.h"

#include <cmath>
#include <ostream>

#include <hesp/exceptions/Exception.h>
#include <hesp/math/Constants.h>

namespace hesp {

//################## CONSTRUCTORS ##################//
Vector2d::Vector2d()
:	x(0), y(0)
{}

Vector2d::Vector2d(double x_, double y_)
:	x(x_), y(y_)
{}

//################## PUBLIC OPERATORS ##################//
Vector2d& Vector2d::operator+=(const Vector2d& rhs)
{
	x += rhs.x;
	y += rhs.y;
	return *this;
}

Vector2d& Vector2d::operator-=(const Vector2d& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	return *this;
}

Vector2d& Vector2d::operator*=(double factor)
{
	x *= factor;
	y *= factor;
	return *this;
}

Vector2d Vector2d::operator-() const
{
	return Vector2d(-x, -y);
}

//################## PUBLIC METHODS ##################//
double Vector2d::distance(const Vector2d& rhs) const
{
	return sqrt(distance_squared(rhs));
}

double Vector2d::distance_squared(const Vector2d& rhs) const
{
	double dx = x - rhs.x;
	double dy = y - rhs.y;
	return dx*dx + dy*dy;
}

double Vector2d::dot(const Vector2d& rhs) const
{
	return x*rhs.x + y*rhs.y;
}

double Vector2d::length() const
{
	return sqrt(length_squared());
}

double Vector2d::length_squared() const
{
	return x*x + y*y;
}

Vector2d& Vector2d::negate()
{
	x = -x;
	y = -y;
	return *this;
}

Vector2d& Vector2d::normalize()
{
	double len = length();
	if(len < SMALL_EPSILON) throw Exception("Unable to normalize vector: too close to zero");
	return (*this) *= 1.0/len;
}

//################## GLOBAL OPERATORS ##################//
Vector2d operator+(const Vector2d& lhs, const Vector2d& rhs)
{
	Vector2d copy = lhs;
	copy += rhs;
	return copy;
}

Vector2d operator-(const Vector2d& lhs, const Vector2d& rhs)
{
	Vector2d copy = lhs;
	copy -= rhs;
	return copy;
}

Vector2d operator*(double factor, const Vector2d& v)
{
	Vector2d copy = v;
	copy *= factor;
	return copy;
}

Vector2d operator*(const Vector2d& v, double factor)
{
	Vector2d copy = v;
	copy *= factor;
	return copy;
}

std::ostream& operator<<(std::ostream& os, const Vector2d& rhs)
{
	os << '(' << rhs.x << ',' << rhs.y << ')';
	return os;
}

}
