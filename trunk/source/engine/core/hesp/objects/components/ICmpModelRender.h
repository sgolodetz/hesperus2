/***
 * hesperus: ICmpModelRender.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICMPMODELRENDER
#define H_HESP_ICMPMODELRENDER

#include "ICmpRender.h"

namespace hesp {

class ICmpModelRender : public ICmpRender
{
	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void render_first_person() const = 0;
	virtual void set_highlights(bool enabled) = 0;
	virtual void update_animation(int milliseconds) = 0;

	//#################### PUBLIC METHODS ####################
public:
	std::string own_type() const			{ return "ModelRender"; }
	static std::string static_own_type()	{ return "ModelRender"; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ICmpModelRender> ICmpModelRender_Ptr;
typedef shared_ptr<const ICmpModelRender> ICmpModelRender_CPtr;

}

#endif
