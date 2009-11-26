/***
 * hesperus: Vector3.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_VECTOR3
#define H_HESP_VECTOR3

#include <ostream>
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

template <typename T>
struct Vector3
{
	//################## PUBLIC VARIABLES ##################//
	T x, y, z;

	//################## CONSTRUCTORS ##################//
	Vector3();
	Vector3(double x_, double y_, double z_);
	Vector3(const std::vector<std::string>& components);

	//################## PUBLIC OPERATORS ##################//
	Vector3& operator+=(const Vector3& rhs);
	Vector3& operator-=(const Vector3& rhs);
	Vector3& operator*=(double factor);
	Vector3& operator/=(double factor);
	Vector3 operator-() const;

	//################## PUBLIC METHODS ##################//
	Vector3 cross(const Vector3& rhs) const;
	double distance(const Vector3& rhs) const;
	double distance_squared(const Vector3& rhs) const;
	double dot(const Vector3& rhs) const;
	double length() const;
	double length_squared() const;
	Vector3& negate();
	Vector3& normalize();
	Vector3 project_onto(const Vector3& rhs) const;
};

//################## GLOBAL OPERATORS ##################//
template <typename T> Vector3<T> operator+(const Vector3<T>& lhs, const Vector3<T>& rhs);
template <typename T> Vector3<T> operator-(const Vector3<T>& lhs, const Vector3<T>& rhs);
template <typename T> Vector3<T> operator*(double factor, const Vector3<T>& v);
template <typename T> Vector3<T> operator*(const Vector3<T>& v, double factor);
template <typename T> Vector3<T> operator/(const Vector3<T>& v, double factor);
template <typename T> std::ostream& operator<<(std::ostream& os, const Vector3<T>& v);
template <typename T> std::istream& operator>>(std::istream& is, Vector3<T>& v);

//################## TYPEDEFS ##################//
typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;

typedef shared_ptr<Vector3d> Vector3d_Ptr;

}

#endif
