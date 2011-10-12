/***
 * hesperus: Bone.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_BONE
#define H_HESP_BONE

#include <string>

#include <hesp/math/vectors/Vector3.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class RBTMatrix> RBTMatrix_Ptr;
typedef shared_ptr<const class RBTMatrix> RBTMatrix_CPtr;

//#################### TYPEDEFS ####################
typedef shared_ptr<class Bone> Bone_Ptr;
typedef shared_ptr<const class Bone> Bone_CPtr;

class Bone
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::string m_name;
	Vector3d m_basePosition;
	RBTMatrix_Ptr m_baseRotation;
	Bone_Ptr m_parent;

	RBTMatrix_Ptr m_absoluteMatrix;		// a temporary matrix for use by BoneHierarchy when configuring poses
	RBTMatrix_Ptr m_relativeMatrix;		// ditto

	//#################### CONSTRUCTORS ####################
public:
	Bone(const std::string& name, const Vector3d& position, const Vector3d& rotationAxis, double rotationAngle);

	//#################### PUBLIC METHODS ####################
public:
	RBTMatrix_Ptr& absolute_matrix();
	const Vector3d& base_position() const;
	const RBTMatrix_CPtr base_rotation() const;
	const std::string& name() const;
	Bone_Ptr parent();
	Bone_CPtr parent() const;
	RBTMatrix_Ptr& relative_matrix();
	void set_parent(const Bone_Ptr& parent);
};

}

#endif
