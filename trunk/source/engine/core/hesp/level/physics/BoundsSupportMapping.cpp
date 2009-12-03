/***
 * hesperus: BoundsSupportMapping.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "BoundsSupportMapping.h"

#include <hesp/level/bounds/Bounds.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
BoundsSupportMapping::BoundsSupportMapping(const Bounds_CPtr& bounds)
:	m_bounds(bounds)
{}

//#################### PUBLIC OPERATORS ####################
Vector3d BoundsSupportMapping::operator()(const Vector3d& n) const
{
	return m_bounds->support_point(n);
}

}
