/***
 * hesperus: ICmpOrientation.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICMPORIENTATION
#define H_HESP_ICMPORIENTATION

#include <hesp/objects/base/ObjectComponent.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class NUVAxes> NUVAxes_Ptr;
typedef shared_ptr<const class NUVAxes> NUVAxes_CPtr;

class ICmpOrientation : public ObjectComponent
{
	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual NUVAxes_Ptr nuv_axes() = 0;
	virtual NUVAxes_CPtr nuv_axes() const = 0;

	//#################### PUBLIC METHODS ####################
public:
	std::string group_type() const			{ return "Orientation"; }
	static std::string static_group_type()	{ return "Orientation"; }

	std::string own_type() const			{ return "Orientation"; }
	static std::string static_own_type()	{ return "Orientation"; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ICmpOrientation> ICmpOrientation_Ptr;
typedef shared_ptr<const ICmpOrientation> ICmpOrientation_CPtr;

}

#endif
