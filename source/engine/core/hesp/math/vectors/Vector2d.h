/***
 * hesperus: Vector2d.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_VECTOR2D
#define H_HESP_VECTOR2D

#include <iosfwd>

namespace hesp {

struct Vector2d
{
	//################## PUBLIC VARIABLES ##################//
	double x, y;

	//################## CONSTRUCTORS ##################//
	Vector2d();
	Vector2d(double x_, double y_);

	//################## PUBLIC OPERATORS ##################//
	Vector2d& operator+=(const Vector2d& rhs);
	Vector2d& operator-=(const Vector2d& rhs);
	Vector2d& operator*=(double factor);
	Vector2d operator-() const;

	//################## PUBLIC METHODS ##################//
	double distance(const Vector2d& rhs) const;
	double distance_squared(const Vector2d& rhs) const;
	double dot(const Vector2d& rhs) const;
	double length() const;
	double length_squared() const;
	Vector2d& negate();
	Vector2d& normalize();
};

//################## GLOBAL OPERATORS ##################//
Vector2d operator+(const Vector2d& lhs, const Vector2d& rhs);
Vector2d operator-(const Vector2d& lhs, const Vector2d& rhs);
Vector2d operator*(double factor, const Vector2d& v);
Vector2d operator*(const Vector2d& v, double factor);
std::ostream& operator<<(std::ostream& os, const Vector2d& rhs);

}

#endif
