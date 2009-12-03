/***
 * hesperus: MathUtil.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MATHUTIL
#define H_HESP_MATHUTIL

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class RBTMatrix> RBTMatrix_Ptr;
typedef shared_ptr<const class RBTMatrix> RBTMatrix_CPtr;
typedef shared_ptr<class RBTQuaternion> RBTQuaternion_Ptr;
typedef shared_ptr<const class RBTQuaternion> RBTQuaternion_CPtr;

struct MathUtil
{
	//#################### PUBLIC METHODS ####################
	static RBTQuaternion_Ptr rbt_matrix_to_quaternion(const RBTMatrix_CPtr& mat);
	static RBTMatrix_Ptr rbt_quaternion_to_matrix(const RBTQuaternion_CPtr& q);
};

}

#endif
