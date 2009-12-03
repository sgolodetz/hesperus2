/***
 * hesperus: Model.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Model.h"

#include "AnimationController.h"
#include "BoneHierarchy.h"
#include "Mesh.h"
#include "Skeleton.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
Model::Model(const Mesh_Ptr& mesh, const Skeleton_Ptr& skeleton)
:	m_mesh(mesh), m_skeleton(skeleton)
{}

//#################### PUBLIC METHODS ####################
ConfiguredPose_Ptr Model::configure_pose(const AnimationController_CPtr& animController) const
{
	return m_skeleton->bone_hierarchy()->configure_pose(animController->get_pose(), animController->get_pose_modifiers());
}

void Model::render(const ConfiguredPose_CPtr& pose) const
{
	m_skeleton->set_pose(pose);
	m_mesh->skin(m_skeleton);
	m_mesh->render();
}

const Skeleton_Ptr& Model::skeleton()
{
	return m_skeleton;
}

Skeleton_CPtr Model::skeleton() const
{
	return m_skeleton;
}

}
