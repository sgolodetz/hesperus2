/***
 * hesperus: SegmentSupportMapping.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "SegmentSupportMapping.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
SegmentSupportMapping::SegmentSupportMapping(const Vector3d& endpoint0, const Vector3d& endpoint1)
:	m_endpoint0(endpoint0), m_endpoint1(endpoint1)
{}

//#################### PUBLIC OPERATORS ####################
Vector3d SegmentSupportMapping::operator()(const Vector3d& n) const
{
	Vector3d dir = m_endpoint1 - m_endpoint0;
	double dotProd = dir.dot(n);
	if(dotProd >= 0) return m_endpoint1;
	else return m_endpoint0;
}

}
