/***
 * hesperus: ConfiguredBone.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ConfiguredBone.h"

#include <hesp/exceptions/Exception.h>
#include <hesp/math/matrices/RBTMatrix.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
ConfiguredBone::ConfiguredBone(const RBTMatrix_CPtr& absoluteMatrix)
:	m_absoluteMatrix(absoluteMatrix)
{
	if(absoluteMatrix == NULL) throw Exception("The absolute matrix of a configured bone must be non-null");
}

//#################### PUBLIC METHODS ####################
const RBTMatrix_CPtr& ConfiguredBone::absolute_matrix() const
{
	return m_absoluteMatrix;
}

Vector3d ConfiguredBone::orientation() const
{
	// Extract the bone orientation from the absolute matrix and return it.
	const RBTMatrix& m = *m_absoluteMatrix;
	return Vector3d(m(0,1), m(1,1), m(2,1));
}

Vector3d ConfiguredBone::position() const
{
	// Extract the bone position from the absolute matrix and return it.
	const RBTMatrix& m = *m_absoluteMatrix;
	return Vector3d(m(0,3), m(1,3), m(2,3));
}

}
