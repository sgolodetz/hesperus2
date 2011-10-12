/***
 * hesperus: VectorUtil.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_VECTORUTIL
#define H_HESP_VECTORUTIL

#include "Vector3.h"

namespace hesp {

Vector3d rotate_about_axis(const Vector3d& v, double degreeAngle, const Vector3d& axis);

}

#endif
