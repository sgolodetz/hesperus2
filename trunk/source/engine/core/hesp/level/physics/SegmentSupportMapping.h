/***
 * hesperus: SegmentSupportMapping.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_SEGMENTSUPPORTMAPPING
#define H_HESP_SEGMENTSUPPORTMAPPING

#include "SupportMapping.h"

namespace hesp {

/**
This class represents the support mapping of a line segment.
*/
class SegmentSupportMapping : public SupportMapping
{
	//#################### PRIVATE VARIABLES ####################
private:
	Vector3d m_endpoint0, m_endpoint1;

	//#################### CONSTRUCTORS ####################
public:
	SegmentSupportMapping(const Vector3d& endpoint0, const Vector3d& endpoint1);

	//#################### PUBLIC OPERATORS ####################
public:
	Vector3d operator()(const Vector3d& n) const;
};

}

#endif
