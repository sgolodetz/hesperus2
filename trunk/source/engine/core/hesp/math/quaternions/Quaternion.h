/***
 * hesperus: Quaternion.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_QUATERNION
#define H_HESP_QUATERNION

#include <iosfwd>

#include <hesp/math/vectors/Vector3.h>

namespace hesp {

struct Quaternion
{
	//#################### PUBLIC VARIABLES ####################
	double w, x, y, z;

	//#################### CONSTRUCTORS ####################
	Quaternion();
	Quaternion(double w_, double x_, double y_, double z_);

	//#################### STATIC FACTORY METHODS ####################
	static Quaternion from_axis_angle(Vector3d axis, double angle);

	//#################### PUBLIC OPERATORS ####################
	Quaternion& operator+=(const Quaternion& rhs);
	Quaternion& operator*=(const Quaternion& rhs);
	Quaternion& operator*=(double scale);

	//#################### PUBLIC METHODS ####################
	Vector3d apply_rotation(const Vector3d& p) const;
	Vector3d apply_unit_rotation(const Vector3d& p) const;
	Quaternion conjugate() const;
	double dot(const Quaternion& rhs) const;
	Quaternion inverse() const;
	double length() const;
	double length_squared() const;
	static Quaternion lerp(const Quaternion& q1, const Quaternion& q2, double t);
	Quaternion& normalize();
	static Quaternion slerp(const Quaternion& q1, const Quaternion& q2, double t);
};

//#################### GLOBAL OPERATORS ####################
Quaternion operator*(const Quaternion& q1, const Quaternion& q2);
Quaternion operator*(const Quaternion& q, double scale);
Quaternion operator*(double scale, const Quaternion& q);
std::ostream& operator<<(std::ostream& os, const Quaternion& q);

}

#endif
