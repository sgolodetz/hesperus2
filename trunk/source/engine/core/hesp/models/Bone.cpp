/***
 * hesperus: Bone.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Bone.h"

#include <hesp/math/matrices/RBTMatrix.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
Bone::Bone(const std::string& name, const Vector3d& position, const Vector3d& rotationAxis, double rotationAngle)
:	m_name(name), m_basePosition(position)
{
	m_baseRotation = RBTMatrix::from_axis_angle_translation(rotationAxis, rotationAngle, Vector3d(0,0,0));
}

//#################### PUBLIC METHODS ####################
RBTMatrix_Ptr& Bone::absolute_matrix()				{ return m_absoluteMatrix; }
const Vector3d& Bone::base_position() const			{ return m_basePosition; }
const RBTMatrix_CPtr Bone::base_rotation() const	{ return m_baseRotation; }
const std::string& Bone::name() const				{ return m_name; }
Bone_Ptr Bone::parent()								{ return m_parent; }
Bone_CPtr Bone::parent() const						{ return m_parent; }
RBTMatrix_Ptr& Bone::relative_matrix()				{ return m_relativeMatrix; }
void Bone::set_parent(const Bone_Ptr& parent)		{ m_parent = parent; }

}
