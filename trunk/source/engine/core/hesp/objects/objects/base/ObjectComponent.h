/***
 * hesperus: ObjectComponent.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_OBJECTCOMPONENT
#define H_HESP_OBJECTCOMPONENT

#include "IObjectComponent.h"

namespace hesp {

class ObjectComponent : public IObjectComponent
{
	//#################### PROTECTED METHODS ####################
protected:
	template <typename C> void check_dependency() const;
};

}

#include "ObjectComponent.tpp"

#endif
