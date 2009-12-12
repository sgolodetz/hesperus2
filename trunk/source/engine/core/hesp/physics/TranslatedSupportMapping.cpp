/***
 * hesperus: TranslatedSupportMapping.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "TranslatedSupportMapping.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
TranslatedSupportMapping::TranslatedSupportMapping(const SupportMapping_CPtr& base, const Vector3d& trans)
:	m_base(base), m_trans(trans)
{}

//#################### PUBLIC OPERATORS ####################
Vector3d TranslatedSupportMapping::operator()(const Vector3d& n) const
{
	// See p.168 of Game Programming Gems 7.
	return (*m_base)(n) + m_trans;
}

}
