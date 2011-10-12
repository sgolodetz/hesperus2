/***
 * hesperus: ModelVertex.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ModelVertex.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
ModelVertex::ModelVertex(const Vector3d& position, const Vector3d& normal)
:	m_position(position), m_normal(normal)
{}

//#################### PUBLIC METHODS ####################
void ModelVertex::add_bone_weight(const BoneWeight& boneWeight)
{
	m_boneWeights.push_back(boneWeight);
}

const std::vector<BoneWeight>& ModelVertex::bone_weights() const
{
	return m_boneWeights;
}

const Vector3d& ModelVertex::position() const
{
	return m_position;
}

}
