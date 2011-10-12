/***
 * hesperus: BoneHierarchy.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "BoneHierarchy.h"

#include <hesp/exceptions/Exception.h>
#include <hesp/math/matrices/RBTMatrix.h>
#include "Bone.h"
#include "ConfiguredBone.h"
#include "ConfiguredPose.h"
#include "Pose.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
BoneHierarchy::BoneHierarchy(const std::vector<Bone_Ptr>& bones)
:	m_bones(bones)
{
	int boneCount = bone_count();
	for(int i=0; i<boneCount; ++i)
	{
		m_boneLookup.insert(std::make_pair(bones[i]->name(), i));
	}
}

//#################### PUBLIC METHODS ####################
void BoneHierarchy::attach_to_parent(const BoneHierarchy_Ptr& parent, const std::string& parentBoneName)
{
	Bone_Ptr parentBone = parent->bones(parentBoneName);
	bones("root")->set_parent(parentBone);
}

int BoneHierarchy::bone_count() const
{
	return static_cast<int>(m_bones.size());
}

Bone_Ptr BoneHierarchy::bones(int i)
{
	return m_bones[i];
}

Bone_CPtr BoneHierarchy::bones(int i) const
{
	return m_bones[i];
}

Bone_Ptr BoneHierarchy::bones(const std::string& name)
{
	return m_bones[find_bone(name)];
}

Bone_CPtr BoneHierarchy::bones(const std::string& name) const
{
	return m_bones[find_bone(name)];
}

ConfiguredPose_Ptr BoneHierarchy::configure_pose(const Pose_CPtr& unconfiguredPose,
												 const std::map<std::string,PoseModifier>& modifiers) const
{
	calculate_relative_matrices(unconfiguredPose->relative_bone_matrices());
	calculate_absolute_matrices(modifiers);

	int boneCount = bone_count();
	std::vector<ConfiguredBone_CPtr> configuredBones(boneCount);
	for(int i=0; i<boneCount; ++i)
	{
		configuredBones[i].reset(new ConfiguredBone(m_bones[i]->absolute_matrix()));
	}

	return ConfiguredPose_Ptr(new ConfiguredPose(configuredBones));
}

void BoneHierarchy::detach_from_parent()
{
	bones("root")->set_parent(Bone_Ptr());
}

int BoneHierarchy::find_bone(const std::string& name) const
{
	std::map<std::string,int>::const_iterator it = m_boneLookup.find(name);
	if(it != m_boneLookup.end()) return it->second;
	else throw Exception("Bone " + name + " does not exist");
}

//#################### PRIVATE METHODS ####################
void BoneHierarchy::calculate_absolute_matrices(const std::map<std::string,PoseModifier>& modifiers) const
{
	int boneCount = bone_count();

	// Step 1: Clear all the existing absolute matrices.
	for(int i=0; i<boneCount; ++i)
	{
		m_bones[i]->absolute_matrix().reset();
	}

	// Step 2: Calculate the new absolute matrices.
	for(int i=0; i<boneCount; ++i)
	{
		calculate_absolute_matrix(m_bones[i], modifiers);
	}
}

void BoneHierarchy::calculate_absolute_matrix(const Bone_Ptr& bone, const std::map<std::string,PoseModifier>& modifiers) const
{
	Bone_Ptr parent = bone->parent();
	if(parent)
	{
		if(!parent->absolute_matrix()) calculate_absolute_matrix(parent, modifiers);
		bone->absolute_matrix() = parent->absolute_matrix() * bone->relative_matrix();
	}
	else
	{
		bone->absolute_matrix() = RBTMatrix::copy(bone->relative_matrix());
	}

	// Handle any pose modifiers applied to this bone.
	std::map<std::string,PoseModifier>::const_iterator it = modifiers.find(bone->name());
	if(it != modifiers.end())
	{
		// This bone has a pose modifier applied to it, so update the absolute matrix accordingly.
		const PoseModifier& modifier = it->second;

		// Calculate the required rotation axis in the frame of the bone.
		Vector3d transformedAxis = bone->absolute_matrix()->inverse()->apply_to_vector(modifier.axis);

		// Construct an appropriate rotation matrix and use it to post-multiply the bone's absolute matrix.
		// Note that if we needed the relative matrix again, we'd need to post-multiply that as well, but
		// in practice this isn't necessary.
		RBTMatrix_Ptr rot = RBTMatrix::from_axis_angle_translation(transformedAxis, modifier.angle, Vector3d(0,0,0));
		bone->absolute_matrix() *= rot;
	}
}

void BoneHierarchy::calculate_relative_matrices(const std::vector<RBTMatrix_CPtr>& unconfiguredRelMats) const
{
	int boneCount = bone_count();
	for(int i=0; i<boneCount; ++i)
	{
		Bone_Ptr bone = m_bones[i];

		if(bone->name() == "root" && bone->parent() != NULL)
		{
			// If this is the root bone of an attached model, its absolute matrix should be
			// the same as the bone to which it's attached: to ensure this, we explicitly
			// set its relative matrix to the identity matrix. Note that only the root bone
			// of each model should be called "root", or this will cause problems.
			bone->relative_matrix() = RBTMatrix::identity();
		}
		else
		{
			RBTMatrix_CPtr baseRot = bone->base_rotation();
			RBTMatrix_Ptr rot = RBTMatrix::copy(unconfiguredRelMats[i]);
			(*rot)(0,3) = (*rot)(1,3) = (*rot)(2,3) = 0;

			RBTMatrix_Ptr& configuredRelMat = bone->relative_matrix();
			configuredRelMat = baseRot * rot;

			const Vector3d& basePos = bone->base_position();
			Vector3d trans((*unconfiguredRelMats[i])(0,3), (*unconfiguredRelMats[i])(1,3), (*unconfiguredRelMats[i])(2,3));
			(*configuredRelMat)(0,3) = basePos.x + trans.x;
			(*configuredRelMat)(1,3) = basePos.y + trans.y;
			(*configuredRelMat)(2,3) = basePos.z + trans.z;
		}
	}
}

}
