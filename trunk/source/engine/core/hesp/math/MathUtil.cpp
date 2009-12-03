/***
 * hesperus: MathUtil.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "MathUtil.h"

#include <cmath>

#include <hesp/math/matrices/RBTMatrix.h>
#include <hesp/math/quaternions/RBTQuaternion.h>
#include "Constants.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
RBTQuaternion_Ptr MathUtil::rbt_matrix_to_quaternion(const RBTMatrix_CPtr& mat)
{
	// For an explanation of how this works, see either of the following links:
	//
	// www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/index.htm
	// www.j3d.org/matrix_faq/matrfaq_latest.html#Q55

	const RBTMatrix& m = *mat;

	// Extract the translation directly from the RBT matrix.
	Vector3d trans(m(0,3), m(1,3), m(2,3));

	// Extract the rotation quaternion from the rest of the RBT matrix.

	/*
	m00 = 1 - 2y^2 - 2z^2	m01 = 2xy - 2wz			m02 = 2xz + 2wy			0
	m10 = 2xy + 2wz			m11 = 1 - 2x^2 - 2z^2	m12 = 2yz - 2wx			0
	m20 = 2xz - 2wy			m21 = 2yz + 2wx			m22 = 1 - 2x^2 - 2y^2	0
	0						0						0						1

	trace = m00 + m11 + m22 + 1
	      = 1 - 2y^2 - 2z^2 + 1 - 2x^2 - 2z^2 + 1 - 2x^2 - 2y^2 + 1
	      = 4(1 - x^2 - y^2 - z^2)
	      = 4w^2

	If trace >= 0 (in practice we use > SMALL_EPSILON for better numerical stability), then:

	w = sqrt(trace)/2
	x = (m21 - m12) / 4w = [(2yz + 2wx) - (2yz - 2wx)] / 4w = 4wx/4w
	y = (m02 - m20) / 4w = [(2xz + 2wy) - (2xz - 2wy)] / 4w = 4wy/4w
	z = (m10 - m01) / 4w = [(2xy + 2wz) - (2xy - 2wz)] / 4w = 4wz/4w

	If trace < 0, we instead extract one of {x,y,z} from the diagonal elements, and use it
	to find the other quaternion components. We pick which one depending on the relative
	sizes of m00, m11 and m22 (if m00 is largest, we extract x from the diagonal, etc.)
	*/

	Quaternion rot;

	double trace = m(0,0) + m(1,1) + m(2,2) + 1;
	if(trace > SMALL_EPSILON)
	{
		// Extract w first (see above)

		double s = 2 * sqrt(trace);	// = 4w
		rot.w = s/4;
		rot.x = (m(2,1) - m(1,2)) / s;
		rot.y = (m(0,2) - m(2,0)) / s;
		rot.z = (m(1,0) - m(0,1)) / s;
	}
	else if(m(0,0) >= m(1,1) && m(0,0) >= m(2,2))
	{
		/*
		Extract x first:

		t = m00 - m11 - m22 + 1
		  = (1 - 2y^2 - 2z^2) - (1 - 2x^2 - 2z^2) - (1 - 2x^2 - 2y^2) + 1
		  = 4x^2

		w = (m21 - m12) / 4x = 4wx/4x
		x = sqrt(t)/2
		y = (m10 + m01) / 4x = [(2xy + 2wz) + (2xy - 2wz)] / 4x = 4xy/4x
		z = (m02 + m20) / 4x = [(2xz + 2wy) + (2xz - 2wy)] / 4x = 4xz/4x
		*/

		double s = 2 * sqrt(m(0,0) - m(1,1) - m(2,2) + 1);	// = 4x
		rot.w = (m(2,1) - m(1,2)) / s;
		rot.x = s/4;
		rot.y = (m(1,0) + m(0,1)) / s;
		rot.z = (m(0,2) + m(2,0)) / s;
	}
	else if(m(1,1) >= m(2,2))
	{
		/*
		Extract y first:

		t = m11 - m00 - m22 + 1
		  = (1 - 2x^2 - 2z^2) - (1 - 2y^2 - 2z^2) - (1 - 2x^2 - 2y^2) + 1
		  = 4y^2

		w = (m02 - m20) / 4y = [(2xz + 2wy) - (2xz - 2wy)] / 4y = 4wy/4y
		x = (m10 + m01) / 4y = 4xy/4y
		y = sqrt(t)/2
		z = (m21 + m12) / 4y = [(2yz + 2wx) + (2yz - 2wx)] / 4y = 4yz/4y
		*/

		double s = 2 * sqrt(m(1,1) - m(0,0) - m(2,2) + 1);	// = 4y
		rot.w = (m(0,2) - m(2,0)) / s;
		rot.x = (m(1,0) + m(0,1)) / s;
		rot.y = s/4;
		rot.z = (m(2,1) + m(1,2)) / s;
	}
	else
	{
		/*
		Extract z first:

		t = m22 - m00 - m11 + 1
		  = (1 - 2x^2 - 2y^2) - (1 - 2y^2 - 2z^2) - (1 - 2x^2 - 2z^2) + 1
		  = 4z^2

		w = (m10 - m01) / 4z = [(2xy + 2wz) - (2xy - 2wz)] / 4z = 4wz/4z
		x = (m02 + m20) / 4z = 4xz/4z
		y = (m21 + m12) / 4z = 4yz/4z
		z = sqrt(t)/2
		*/

		double s = 2 * sqrt(m(2,2) - m(0,0) - m(1,1) + 1);	// = 4z
		rot.w = (m(1,0) - m(0,1)) / s;
		rot.x = (m(0,2) + m(2,0)) / s;
		rot.y = (m(2,1) + m(1,2)) / s;
		rot.z = s/4;
	}

	return RBTQuaternion_Ptr(new RBTQuaternion(rot, trans));
}

RBTMatrix_Ptr MathUtil::rbt_quaternion_to_matrix(const RBTQuaternion_CPtr& q)
{
	// FIXME: Optimize this as per www.gamasutra.com/features/19980703/quaternions_01.htm.
	RBTMatrix_Ptr ret = RBTMatrix::zeros();
	RBTMatrix& m = *ret;

	const Quaternion& rot = q->rotation();
	const Vector3d& trans = q->translation();

	// The quaternion for the rotation should be normalized.
	assert(fabs(rot.length_squared() - 1) < SMALL_EPSILON);

	const double& w = rot.w;
	const double& x = rot.x;
	const double& y = rot.y;
	const double& z = rot.z;

	// See p.72 of "Mathematics for 3D Game Programming & Computer Graphics" (Eric Lengyel).
	m(0,0) = 1 - 2*y*y - 2*z*z;		m(0,1) = 2*x*y - 2*w*z;			m(0,2) = 2*x*z + 2*w*y;			m(0,3) = trans.x;
	m(1,0) = 2*x*y + 2*w*z;			m(1,1) = 1 - 2*x*x - 2*z*z;		m(1,2) = 2*y*z - 2*w*x;			m(1,3) = trans.y;
	m(2,0) = 2*x*z - 2*w*y;			m(2,1) = 2*y*z + 2*w*x;			m(2,2) = 1 - 2*x*x - 2*y*y;		m(2,3) = trans.z;

	return ret;
}

}
