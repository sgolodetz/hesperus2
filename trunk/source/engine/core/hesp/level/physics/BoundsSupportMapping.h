/***
 * hesperus: BoundsSupportMapping.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_BOUNDSSUPPORTMAPPING
#define H_HESP_BOUNDSSUPPORTMAPPING

#include "SupportMapping.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<const class Bounds> Bounds_CPtr;

class BoundsSupportMapping : public SupportMapping
{
	//#################### PRIVATE VARIABLES ####################
private:
	Bounds_CPtr m_bounds;

	//#################### CONSTRUCTORS ####################
public:
	explicit BoundsSupportMapping(const Bounds_CPtr& bounds);

	//#################### PUBLIC OPERATORS ####################
public:
	Vector3d operator()(const Vector3d& n) const;
};

}

#endif
