/***
 * hesperus: BoneWeight.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_BONEWEIGHT
#define H_HESP_BONEWEIGHT

namespace hesp {

class BoneWeight
{
	//#################### PRIVATE VARIABLES ####################
private:
	int m_boneIndex;
	double m_weight;	// in the range [0,1]

	//#################### CONSTRUCTORS ####################
public:
	BoneWeight(int boneIndex, double weight);

	//#################### PUBLIC METHODS ####################
public:
	int bone_index() const;
	double weight() const;
};

}

#endif
