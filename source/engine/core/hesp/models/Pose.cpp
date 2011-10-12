/***
 * hesperus: Pose.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Pose.h"

#include <hesp/math/MathUtil.h>
#include <hesp/math/quaternions/RBTQuaternion.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
Pose::Pose(const std::vector<RBTMatrix_CPtr>& relativeBoneMatrices)
:	m_relativeBoneMatrices(relativeBoneMatrices)
{}

//#################### PUBLIC METHODS ####################
Pose_Ptr Pose::interpolate(const Pose_CPtr& lhs, const Pose_CPtr& rhs, double t)
{
	const std::vector<RBTMatrix_CPtr>& lhsBoneMatrices = lhs->relative_bone_matrices();
	const std::vector<RBTMatrix_CPtr>& rhsBoneMatrices = rhs->relative_bone_matrices();

	assert(lhsBoneMatrices.size() == rhsBoneMatrices.size());

	int boneCount = static_cast<int>(lhsBoneMatrices.size());
	std::vector<RBTMatrix_CPtr> boneMatrices(boneCount);

	for(int i=0; i<boneCount; ++i)
	{
		const RBTMatrix_CPtr& m1 = lhsBoneMatrices[i];
		const RBTMatrix_CPtr& m2 = rhsBoneMatrices[i];
		RBTQuaternion_Ptr q1 = MathUtil::rbt_matrix_to_quaternion(m1);
		RBTQuaternion_Ptr q2 = MathUtil::rbt_matrix_to_quaternion(m2);
		RBTQuaternion_Ptr inter = RBTQuaternion::interpolate(q1, q2, t);
		boneMatrices[i] = MathUtil::rbt_quaternion_to_matrix(inter);
	}

	return Pose_Ptr(new Pose(boneMatrices));
}

const std::vector<RBTMatrix_CPtr>& Pose::relative_bone_matrices() const
{
	return m_relativeBoneMatrices;
}

}
