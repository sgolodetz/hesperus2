/***
 * hesperus: MinkDiffSupportMapping.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "MinkDiffSupportMapping.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
MinkDiffSupportMapping::MinkDiffSupportMapping(const SupportMapping_CPtr& lhs, const SupportMapping_CPtr& rhs)
:	m_lhs(lhs), m_rhs(rhs)
{}

//#################### PUBLIC METHODS ####################
Vector3d MinkDiffSupportMapping::operator()(const Vector3d& n) const
{
	/*
	See p.171 of Game Programming Gems 7, where this appears as:

	S_{B-A}(n) = S_B(n) - S_A(-n)
	*/
	return (*m_lhs)(n) - (*m_rhs)(-n);
}

}
