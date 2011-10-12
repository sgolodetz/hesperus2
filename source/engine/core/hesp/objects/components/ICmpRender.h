/***
 * hesperus: ICmpRender.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICMPRENDER
#define H_HESP_ICMPRENDER

#include <hesp/objects/base/ObjectComponent.h>

namespace hesp {

class ICmpRender : public ObjectComponent
{
	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void render() const = 0;

	//#################### PUBLIC METHODS ####################
public:
	std::string group_type() const			{ return "Render"; }
	static std::string static_group_type()	{ return "Render"; }

	std::string own_type() const			{ return "Render"; }
	static std::string static_own_type()	{ return "Render"; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ICmpRender> ICmpRender_Ptr;
typedef shared_ptr<const ICmpRender> ICmpRender_CPtr;

}

#endif
