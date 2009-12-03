/***
 * hesperus: RBTMatrix.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_RBTMATRIX
#define H_HESP_RBTMATRIX

#include <hesp/math/vectors/Vector3.h>

namespace hesp {

//#################### TYPEDEFS ####################
typedef shared_ptr<class RBTMatrix> RBTMatrix_Ptr;
typedef shared_ptr<const class RBTMatrix> RBTMatrix_CPtr;

/**
This class represents rigid-body transformation matrices.
*/
class RBTMatrix
{
	//#################### PRIVATE VARIABLES ####################
private:
	double m[3][4];

	//#################### CONSTRUCTORS ####################
private:
	RBTMatrix();

	//#################### STATIC FACTORY METHODS ####################
public:
	static RBTMatrix_Ptr from_axis_angle_translation(Vector3d axis, double angle, const Vector3d& translation);
	static RBTMatrix_Ptr identity();
	static RBTMatrix_Ptr zeros();

	//#################### PUBLIC OPERATORS ####################
public:
	double& operator()(int i, int j);
	double operator()(int i, int j) const;

	//#################### PUBLIC METHODS ####################
public:
	void add_scaled(const RBTMatrix_CPtr& mat, double scale);
	Vector3d apply_to_point(const Vector3d& p) const;
	Vector3d apply_to_vector(const Vector3d& v) const;
	static RBTMatrix_Ptr copy(const RBTMatrix_CPtr& rhs);
	RBTMatrix_Ptr inverse() const;
	std::vector<double> rep() const;
	void reset_to_zeros();
};

//#################### GLOBAL OPERATORS ####################
RBTMatrix_Ptr& operator+=(RBTMatrix_Ptr& lhs, const RBTMatrix_CPtr& rhs);
RBTMatrix_Ptr& operator*=(RBTMatrix_Ptr& lhs, const RBTMatrix_CPtr& rhs);
RBTMatrix_Ptr& operator*=(RBTMatrix_Ptr& lhs, double scale);
RBTMatrix_Ptr operator*(const RBTMatrix_CPtr& lhs, const RBTMatrix_CPtr& rhs);
RBTMatrix_Ptr operator*(const RBTMatrix_CPtr& lhs, double scale);
RBTMatrix_Ptr operator*(double scale, const RBTMatrix_CPtr& rhs);

}

#endif
