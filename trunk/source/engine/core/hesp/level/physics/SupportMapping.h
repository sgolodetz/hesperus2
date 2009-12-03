/***
 * hesperus: SupportMapping.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_SUPPORTMAPPING
#define H_HESP_SUPPORTMAPPING

#include <hesp/math/vectors/Vector3.h>

namespace hesp {

/**
This class represents a support mapping for a physical object.
A support mapping is a function which returns a *consistent*
farthest point on the object in any given direction.
*/
class SupportMapping
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~SupportMapping() {}

	//#################### PUBLIC ABSTRACT OPERATORS ####################
public:
	virtual Vector3d operator()(const Vector3d& n) const = 0;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<SupportMapping> SupportMapping_Ptr;
typedef shared_ptr<const SupportMapping> SupportMapping_CPtr;

}

#endif
