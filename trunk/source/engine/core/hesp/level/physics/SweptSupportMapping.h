/***
 * hesperus: SweptSupportMapping.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_SWEPTSUPPORTMAPPING
#define H_HESP_SWEPTSUPPORTMAPPING

#include "SupportMapping.h"

namespace hesp {

/**
This class represents the support mapping obtained by sweeping out
one object with another.
*/
class SweptSupportMapping : public SupportMapping
{
	//#################### PRIVATE VARIABLES ####################
private:
	SupportMapping_CPtr m_mapping1, m_mapping2;

	//#################### CONSTRUCTORS ####################
public:
	SweptSupportMapping(const SupportMapping_CPtr& mapping1, const SupportMapping_CPtr& mapping2);

	//#################### PUBLIC OPERATORS ####################
public:
	Vector3d operator()(const Vector3d& n) const;
};

}

#endif
