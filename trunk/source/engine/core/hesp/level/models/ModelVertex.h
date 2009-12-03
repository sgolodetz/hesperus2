/***
 * hesperus: ModelVertex.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MODELVERTEX
#define H_HESP_MODELVERTEX

#include <vector>

#include <hesp/math/vectors/Vector3.h>
#include "BoneWeight.h"

namespace hesp {

class ModelVertex
{
	//#################### PRIVATE VARIABLES ####################
private:
	Vector3d m_position;
	Vector3d m_normal;
	std::vector<BoneWeight> m_boneWeights;

	//#################### CONSTRUCTORS ####################
public:
	ModelVertex(const Vector3d& position, const Vector3d& normal);

	//#################### PUBLIC METHODS ####################
public:
	void add_bone_weight(const BoneWeight& boneWeight);
	const std::vector<BoneWeight>& bone_weights() const;
	const Vector3d& position() const;
};

}

#endif
