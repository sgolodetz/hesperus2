/***
 * hesperus: Vector3.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "Vector3.h"

#include <cmath>

#include <boost/lexical_cast.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <hesp/exceptions/Exception.h>
#include <hesp/math/Constants.h>

namespace hesp {

//################## CONSTRUCTORS ##################//
template <typename T>
Vector3<T>::Vector3()
:	x(0), y(0), z(0)
{}

template <typename T>
Vector3<T>::Vector3(double x_, double y_, double z_)
:	x(static_cast<T>(x_)),
	y(static_cast<T>(y_)),
	z(static_cast<T>(z_))
{}

template <typename T>
Vector3<T>::Vector3(const std::vector<std::string>& components)
{
	if(components.size() != 3) throw Exception("Incorrect number of vector components");

	try
	{
		x = lexical_cast<T>(components[0]);
		y = lexical_cast<T>(components[1]);
		z = lexical_cast<T>(components[2]);
	}
	catch(bad_lexical_cast&) { throw Exception("One of the vector components is not a number"); }
}

//################## PUBLIC OPERATORS ##################//
template <typename T>
Vector3<T>& Vector3<T>::operator+=(const Vector3& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}

template <typename T>
Vector3<T>& Vector3<T>::operator-=(const Vector3& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return *this;
}

template <typename T>
Vector3<T>& Vector3<T>::operator*=(double factor)
{
	x *= static_cast<T>(factor);
	y *= static_cast<T>(factor);
	z *= static_cast<T>(factor);
	return *this;
}

template <typename T>
Vector3<T>& Vector3<T>::operator/=(double factor)
{
	x /= static_cast<T>(factor);
	y /= static_cast<T>(factor);
	z /= static_cast<T>(factor);
	return *this;
}

template <typename T>
Vector3<T> Vector3<T>::operator-() const
{
	return Vector3(-x, -y, -z);
}

//################## PUBLIC METHODS ##################//
template <typename T>
Vector3<T> Vector3<T>::cross(const Vector3& rhs) const
{
	return Vector3(y*rhs.z - z*rhs.y,
					z*rhs.x - x*rhs.z,
					x*rhs.y - y*rhs.x);
}

template <typename T>
double Vector3<T>::distance(const Vector3& rhs) const
{
	return sqrt(distance_squared(rhs));
}

template <typename T>
double Vector3<T>::distance_squared(const Vector3& rhs) const
{
	double dx = x - rhs.x;
	double dy = y - rhs.y;
	double dz = z - rhs.z;
	return dx*dx + dy*dy + dz*dz;
}

template <typename T>
double Vector3<T>::dot(const Vector3& rhs) const
{
	return x*rhs.x + y*rhs.y + z*rhs.z;
}

template <typename T>
double Vector3<T>::length() const
{
	return sqrt(length_squared());
}

template <typename T>
double Vector3<T>::length_squared() const
{
	return x*x + y*y + z*z;
}

template <typename T>
Vector3<T>& Vector3<T>::negate()
{
	x = -x;
	y = -y;
	z = -z;
	return *this;
}

template <typename T>
Vector3<T>& Vector3<T>::normalize()
{
	double len = length();
	if(len < SMALL_EPSILON) throw Exception("Unable to normalize vector: too close to zero");
	return (*this) *= 1.0/len;
}

template <typename T>
Vector3<T> Vector3<T>::project_onto(const Vector3& rhs) const
{
	// (lhs . rhs / |rhs|) * (rhs / |rhs|) = rhs * (lhs . rhs / |rhs|^2)
	return rhs * (dot(rhs) / rhs.length_squared());
}

//################## GLOBAL OPERATORS ##################//
template <typename T>
Vector3<T> operator+(const Vector3<T>& lhs, const Vector3<T>& rhs)
{
	Vector3<T> copy = lhs;
	copy += rhs;
	return copy;
}

template <typename T>
Vector3<T> operator-(const Vector3<T>& lhs, const Vector3<T>& rhs)
{
	Vector3<T> copy = lhs;
	copy -= rhs;
	return copy;
}

template <typename T>
Vector3<T> operator*(double factor, const Vector3<T>& v)
{
	Vector3<T> copy = v;
	copy *= factor;
	return copy;
}

template <typename T>
Vector3<T> operator*(const Vector3<T>& v, double factor)
{
	Vector3<T> copy = v;
	copy *= factor;
	return copy;
}

template <typename T>
Vector3<T> operator/(const Vector3<T>& v, double factor)
{
	Vector3<T> copy = v;
	copy /= factor;
	return copy;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector3<T>& v)
{
	os << "( " << v.x << ' ' << v.y << ' ' << v.z << " )";
	return os;
}

template <typename T>
std::istream& operator>>(std::istream& is, Vector3<T>& v)
{
	std::ios_base::fmtflags oldFlags = is.flags();
	is.setf(std::ios_base::skipws);

	std::string dummy;
	is >> dummy >> v.x >> v.y >> v.z >> dummy;

	is.flags(oldFlags);

	return is;
}

//################## EXPLICIT INSTANTIATIONS ##################//
template struct Vector3<float>;
template Vector3f operator+(const Vector3f& lhs, const Vector3f& rhs);
template Vector3f operator-(const Vector3f& lhs, const Vector3f& rhs);
template Vector3f operator*(double factor, const Vector3f& v);
template Vector3f operator*(const Vector3f& v, double factor);
template Vector3f operator/(const Vector3f& v, double factor);
template std::ostream& operator<<(std::ostream& os, const Vector3f& v);
template std::istream& operator>>(std::istream& is, Vector3f& v);

template struct Vector3<double>;
template Vector3d operator+(const Vector3d& lhs, const Vector3d& rhs);
template Vector3d operator-(const Vector3d& lhs, const Vector3d& rhs);
template Vector3d operator*(double factor, const Vector3d& v);
template Vector3d operator*(const Vector3d& v, double factor);
template Vector3d operator/(const Vector3d& v, double factor);
template std::ostream& operator<<(std::ostream& os, const Vector3d& v);
template std::istream& operator>>(std::istream& is, Vector3d& v);

}
