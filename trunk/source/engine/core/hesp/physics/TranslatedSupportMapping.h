/***
 * hesperus: TranslatedSupportMapping.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_TRANSLATEDSUPPORTMAPPING
#define H_HESP_TRANSLATEDSUPPORTMAPPING

#include "SupportMapping.h"

namespace hesp {

class TranslatedSupportMapping : public SupportMapping
{
	//#################### PRIVATE VARIABLES ####################
private:
	SupportMapping_CPtr m_base;
	Vector3d m_trans;

	//#################### CONSTRUCTORS ####################
public:
	TranslatedSupportMapping(const SupportMapping_CPtr& base, const Vector3d& trans);

	//#################### PUBLIC OPERATORS ####################
public:
	Vector3d operator()(const Vector3d& n) const;
};

}

#endif
