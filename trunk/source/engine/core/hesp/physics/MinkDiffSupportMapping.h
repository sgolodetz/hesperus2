/***
 * hesperus: MinkDiffSupportMapping.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MINKDIFFSUPPORTMAPPING
#define H_HESP_MINKDIFFSUPPORTMAPPING

#include "SupportMapping.h"

namespace hesp {

/**
This class represents a support mapping which is the Minkowski
difference of two other support mappings.
*/
class MinkDiffSupportMapping : public SupportMapping
{
	//#################### PRIVATE VARIABLES ####################
private:
	SupportMapping_CPtr m_lhs, m_rhs;

	//#################### CONSTRUCTORS ####################
public:
	MinkDiffSupportMapping(const SupportMapping_CPtr& lhs, const SupportMapping_CPtr& rhs);

	//#################### PUBLIC METHODS ####################
public:
	Vector3d operator()(const Vector3d& n) const;
};

}

#endif
