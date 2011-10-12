/***
 * hesperus: Skeleton.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Skeleton.h"

#include <hesp/ogl/WrappedGL.h>

#include <hesp/exceptions/Exception.h>
#include <hesp/math/matrices/RBTMatrix.h>
#include "BoneHierarchy.h"
#include "ConfiguredBone.h"
#include "ConfiguredPose.h"
#include "Pose.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
Skeleton::Skeleton(const BoneHierarchy_Ptr& boneHierarchy, const std::map<std::string,Animation_CPtr>& animations)
:	m_boneHierarchy(boneHierarchy), m_animations(animations)
{
	set_pose(boneHierarchy->configure_pose(make_rest_pose()));
	build_to_bone_matrices();
}

//#################### PUBLIC METHODS ####################
Animation_CPtr Skeleton::animation(const std::string& name) const
{
	std::map<std::string,Animation_CPtr>::const_iterator it = m_animations.find(name);
	if(it != m_animations.end())
	{
		return it->second;
	}
	else throw Exception("There is no animation named " + name);
}

const BoneHierarchy_Ptr& Skeleton::bone_hierarchy()
{
	return m_boneHierarchy;
}

BoneHierarchy_CPtr Skeleton::bone_hierarchy() const
{
	return m_boneHierarchy;
}

const ConfiguredPose_CPtr& Skeleton::get_pose() const
{
	return m_pose;
}

bool Skeleton::has_animation(const std::string& name) const
{
	return m_animations.find(name) != m_animations.end();
}

Pose_Ptr Skeleton::make_rest_pose() const
{
	int boneCount = m_boneHierarchy->bone_count();
	std::vector<RBTMatrix_CPtr> boneMatrices(boneCount, RBTMatrix::identity());
	return Pose_Ptr(new Pose(boneMatrices));
}

void Skeleton::render_bones() const
{
	// Note:	This is a method used for testing purposes only. It's not guaranteed
	//			to work when hierarches are attached to each other (because then the
	//			parent of a bone may not necessarily be in the same bone hierarchy).
	int boneCount = m_boneHierarchy->bone_count();
	for(int i=0; i<boneCount; ++i)
	{
		const ConfiguredBone_CPtr& configuredBone = m_pose->bones(i);
		Vector3d pos = configuredBone->position();
		Vector3d orient = configuredBone->orientation();
		Vector3d end = pos + orient * 0.1;

		glBegin(GL_LINES);
			glColor3d(1,0,0);	glVertex3d(pos.x, pos.y, pos.z);
			glColor3d(0,1,0);	glVertex3d(end.x, end.y, end.z);
		glEnd();

#if 0
		try
		{
			Bone_CPtr bone = m_boneHierarchy->bones(i);
			if(bone->parent() != NULL)
			{
				int parentIndex = m_boneHierarchy->find_bone(bone->parent()->name());
				if(bone->name() == "foot.l") parentIndex = m_boneHierarchy->find_bone("lower_leg.l");
				if(bone->name() == "foot.r") parentIndex = m_boneHierarchy->find_bone("lower_leg.r");
				const ConfiguredBone_CPtr& parent = m_pose->bones(parentIndex);

				Vector3d parentPos = parent->position();
				glColor3d(1,1,1);
				glBegin(GL_LINES);
					glVertex3d(pos.x, pos.y, pos.z);
					glVertex3d(parentPos.x, parentPos.y, parentPos.z);
				glEnd();
			}
		}
		catch(...) { /* This is deliberately designed to fail silently - the code in the try is non-critical */ }
#endif
	}
}

void Skeleton::set_pose(const ConfiguredPose_CPtr& pose)
{
	m_pose = pose;
}

RBTMatrix_CPtr Skeleton::to_bone_matrix(int i) const
{
	return m_toBoneMatrices[i];
}

//#################### PRIVATE METHODS ####################
void Skeleton::build_to_bone_matrices()
{
	int boneCount = m_boneHierarchy->bone_count();
	m_toBoneMatrices.resize(boneCount);

	for(int i=0; i<boneCount; ++i)
	{
		const RBTMatrix_CPtr& fromBoneMatrix = m_pose->bones(i)->absolute_matrix();
		m_toBoneMatrices[i] = fromBoneMatrix->inverse();
	}
}

}
