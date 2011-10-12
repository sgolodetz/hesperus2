/***
 * hesperus: BoneWeight.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "BoneWeight.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
BoneWeight::BoneWeight(int boneIndex, double weight)
:	m_boneIndex(boneIndex), m_weight(weight)
{}

//#################### PUBLIC METHODS ####################
int BoneWeight::bone_index() const
{
	return m_boneIndex;
}

double BoneWeight::weight() const
{
	return m_weight;
}

}
