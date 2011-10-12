/***
 * hesperus: SweptSupportMapping.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "SweptSupportMapping.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
SweptSupportMapping::SweptSupportMapping(const SupportMapping_CPtr& mapping1, const SupportMapping_CPtr& mapping2)
:	m_mapping1(mapping1), m_mapping2(mapping2)
{}

//#################### PUBLIC OPERATORS ####################
Vector3d SweptSupportMapping::operator()(const Vector3d& n) const
{
	// See p.169 of Game Programming Gems 7.
	return (*m_mapping1)(n) + (*m_mapping2)(n);
}

}
