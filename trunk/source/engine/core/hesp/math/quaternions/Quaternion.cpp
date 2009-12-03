/***
 * hesperus: Quaternion.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Quaternion.h"

#include <cmath>

#include <hesp/exceptions/Exception.h>
#include <hesp/math/Constants.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
Quaternion::Quaternion()
:	w(0), x(0), y(0), z(0)
{}

Quaternion::Quaternion(double w_, double x_, double y_, double z_)
:	w(w_), x(x_), y(y_), z(z_)
{}

//#################### STATIC FACTORY METHODS ####################
Quaternion Quaternion::from_axis_angle(Vector3d axis, double angle)
{
	if(fabs(axis.length_squared() - 1) > SMALL_EPSILON)
	{
		if(axis.length_squared() > EPSILON*EPSILON) axis.normalize();
		else throw Exception("Quaternion::from_axis_angle: Can't rotate about a zero-length axis");
	}

	double cosHalfTheta = cos(angle/2);
	double sinHalfTheta = sqrt(1 - cosHalfTheta*cosHalfTheta);
	return Quaternion(cosHalfTheta, sinHalfTheta * axis.x, sinHalfTheta * axis.y, sinHalfTheta * axis.z);
}

//#################### PUBLIC OPERATORS ####################
Quaternion& Quaternion::operator+=(const Quaternion& rhs)
{
	w += rhs.w; x += rhs.x; y += rhs.y; z += rhs.z;
	return *this;
}

Quaternion& Quaternion::operator*=(const Quaternion& rhs)
{
	// Note:	I've implemented *= in terms of * rather than the other way round
	//			because each of the new values uses many of the old values here,
	//			so we'd otherwise have to cache them.
	*this = *this * rhs;
	return *this;
}

Quaternion& Quaternion::operator*=(double scale)
{
	w *= scale; x *= scale; y *= scale; z *= scale;
	return *this;
}

//#################### PUBLIC METHODS ####################
Vector3d Quaternion::apply_rotation(const Vector3d& p) const
{
	Quaternion quatP(0, p.x, p.y, p.z);
	Quaternion result = *this * quatP * inverse();
	return Vector3d(result.x, result.y, result.z);
}

Vector3d Quaternion::apply_unit_rotation(const Vector3d& p) const
{
	// This method is simply an optimized version of apply_rotation for unit quaternions.
	// Since the inverse of a unit quaternion is its conjugate, we can save some work.
	assert(fabs(length_squared() - 1) < SMALL_EPSILON);
	Quaternion quatP(0, p.x, p.y, p.z);
	Quaternion result = *this * quatP * conjugate();
	return Vector3d(result.x, result.y, result.z);
}

Quaternion Quaternion::conjugate() const
{
	return Quaternion(w, -x, -y, -z);
}

double Quaternion::dot(const Quaternion& rhs) const
{
	return w*rhs.w + x*rhs.x + y*rhs.y + z*rhs.z;
}

Quaternion Quaternion::inverse() const
{
	double k = length_squared();
	return Quaternion(w/k, -x/k, -y/k, -z/k);
}

double Quaternion::length() const
{
	return sqrt(w*w + x*x + y*y + z*z);
}

double Quaternion::length_squared() const
{
	return w*w + x*x + y*y + z*z;
}

Quaternion Quaternion::lerp(const Quaternion& q1, const Quaternion& q2, double t)
{
	return Quaternion(
		(1-t)*q1.w + t*q2.w,
		(1-t)*q1.x + t*q2.x,
		(1-t)*q1.y + t*q2.y,
		(1-t)*q1.z + t*q2.z
	);
}

Quaternion& Quaternion::normalize()
{
	double len = length();
	if(len < SMALL_EPSILON) throw Exception("Unable to normalize quaternion: too close to zero");
	return (*this) *= 1/len;
}

Quaternion Quaternion::slerp(const Quaternion& q1, const Quaternion& q2, double t)
{
	/*
	As per p.75 of "Mathematics for 3D Game Programming & Computer Graphics" (Eric Lengyel),
	the formula for slerp is:

	q(t) = (sin(theta*(1-t)) * q1 + sin(theta*t) * q2) / sin(theta)
	*/

	assert(fabs(q1.length_squared() - 1) < SMALL_EPSILON);
	assert(fabs(q2.length_squared() - 1) < SMALL_EPSILON);

	if(t < 0) return q1;
	if(t > 1) return q2;

	// Calculate the dot product between the quaternions. If it's < 0, then
	// negate either one of them (q2 was arbitrarily picked here). This is
	// valid since q and -q represent the same rotation. Choosing the signs
	// so that q1 . q2 >= 0 is a good idea because it ensures that the
	// interpolation takes place over the shortest path.
	Quaternion q3 = q2;
	double dotProd = q1.dot(q3);
	if(dotProd < 0)
	{
		q3 *= -1;
		dotProd *= -1;
	}

	if(fabs(dotProd - 1) > EPSILON)
	{
		double theta = acos(dotProd);
		double sinTheta = sin(theta);

		Quaternion ret = sin(theta*(1-t)) * q1;
		ret += sin(theta*t) * q2;
		ret *= 1/sinTheta;

		return ret;
	}
	else
	{
		// If the angle theta between q1 and q3 is pretty small, we can't use
		// slerp since dividing by sin theta would give us a divide-by-zero
		// error, so we fall back to using simple lerp, which works fine here.
		// Note that the other special case (where theta is nearly pi) doesn't
		// occur because of the sign flip we do to q2 in this case above.
		Quaternion ret = lerp(q1, q3, t);
		ret.normalize();
		return ret;
	}
}

//#################### GLOBAL OPERATORS ####################
Quaternion operator*(const Quaternion& q1, const Quaternion& q2)
{
#if 1
	/*
	Optimized as per www.gamasutra.com/features/19980703/quaternions_01.htm (with slight modifications).
	Note that additions are generally substantially faster than multiplications.

	- The normal method takes 16 multiplications and 12 additions.
	- The optimized method takes 8 multiplications, 4 divisions and 28 additions.

	A = (w1 + x1) * (w2 + x2) = w1w2 + w1x2 + x1w2 + x1x2
	B = (z1 - y1) * (y2 - z2) = z1y2 - z1z2 - y1y2 + y1z2
	C = (w1 - x1) * (y2 + z2) = w1y2 + w1z2 - x1y2 - x1z2
	D = (y1 + z1) * (w2 - x2) = y1w2 - y1x2 + z1w2 - z1x2
	E = (x1 + z1) * (x2 + y2) = x1x2 + x1y2 + z1x2 + z1y2
	F = (x1 - z1) * (x2 - y2) = x1x2 - x1y2 - z1x2 + z1y2
	G = (w1 + y1) * (w2 - z2) = w1w2 - w1z2 + y1w2 - y1z2
	H = (w1 - y1) * (w2 + z2) = w1w2 + w1z2 - y1w2 - y1z2
	...
	I = (E + F)/2 = x1x2 + z1y2
	J = (E - F)/2 = x1y2 + z1x2
	K = (G + H)/2 = w1w2 - y1z2
	L = (G - H)/2 = -w1z2 + y1w2
	...
	w = B - I + K
	  = (z1y2 - z1z2 - y1y2 + y1z2) - (x1x2 + z1y2) + (w1w2 - y1z2)
	  = w1w2 - x1x2 - y1y2 - z1z2
	x = A - I - K
	  = (w1w2 + w1x2 + x1w2 + x1x2) - (x1x2 + z1y2) - (w1w2 - y1z2)
	  = w1x2 + x1w2 - z1y2 + y1z2
	y = C + J + L
	  = (w1y2 + w1z2 - x1y2 - x1z2) + (x1y2 + z1x2) + (-w1z2 + y1w2)
	  = w1y2 - x1z2 + z1x2 + y1w2
	z = D + J - L
	  = (y1w2 - y1x2 + z1w2 - z1x2) + (x1y2 + z1x2) - (-w1z2 + y1w2)
	  = -y1x2 + z1w2 + x1y2 + w1z2
	*/

	double	A = (q1.w + q1.x) * (q2.w + q2.x),
			B = (q1.z - q1.y) * (q2.y - q2.z),
			C = (q1.w - q1.x) * (q2.y + q2.z),
			D = (q1.y + q1.z) * (q2.w - q2.x),
			E = (q1.x + q1.z) * (q2.x + q2.y),
			F = (q1.x - q1.z) * (q2.x - q2.y),
			G = (q1.w + q1.y) * (q2.w - q2.z),
			H = (q1.w - q1.y) * (q2.w + q2.z);

	double	I = (E + F)/2,
			J = (E - F)/2,
			K = (G + H)/2,
			L = (G - H)/2;

	return Quaternion(B - I + K, A - I - K, C + J + L, D + J - L);
#else
	return Quaternion(
		q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z,
		q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y,
		q1.w*q2.y - q1.x*q2.z + q1.y*q2.w + q1.z*q2.x,
		q1.w*q2.z + q1.x*q2.y - q1.y*q2.x + q1.z*q2.w
	);
#endif
}

Quaternion operator*(const Quaternion& q, double scale)
{
	Quaternion copy(q);
	copy *= scale;
	return copy;
}

Quaternion operator*(double scale, const Quaternion& q)
{
	Quaternion copy(q);
	copy *= scale;
	return copy;
}

std::ostream& operator<<(std::ostream& os, const Quaternion& q)
{
	os << "( " << q.w << ' ' << q.x << ' ' << q.y << ' ' << q.z << " )";
	return os;
}

}
