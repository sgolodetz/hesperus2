/***
 * hesperus: Pose.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_POSE
#define H_HESP_POSE

#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<const class RBTMatrix> RBTMatrix_CPtr;

//#################### TYPEDEFS ####################
typedef shared_ptr<class Pose> Pose_Ptr;
typedef shared_ptr<const class Pose> Pose_CPtr;

class Pose
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::vector<RBTMatrix_CPtr> m_relativeBoneMatrices;

	//#################### CONSTRUCTORS ####################
public:
	explicit Pose(const std::vector<RBTMatrix_CPtr>& relativeBoneMatrices);

	//#################### COPY CONSTRUCTOR & ASSIGNMENT OPERATOR ####################
private:
	Pose(const Pose&);
	Pose& operator=(const Pose&);

	//#################### PUBLIC METHODS ####################
public:
	static Pose_Ptr interpolate(const Pose_CPtr& lhs, const Pose_CPtr& rhs, double t);
	const std::vector<RBTMatrix_CPtr>& relative_bone_matrices() const;
};

}

#endif
