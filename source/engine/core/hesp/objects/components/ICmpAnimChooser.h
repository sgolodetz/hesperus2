/***
 * hesperus: ICmpAnimChooser.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICMPANIMCHOOSER
#define H_HESP_ICMPANIMCHOOSER

#include <hesp/objects/base/ObjectComponent.h>

namespace hesp {

class ICmpAnimChooser : public ObjectComponent
{
	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual std::string choose_animation() = 0;

	//#################### PUBLIC METHODS ####################
public:
	std::string group_type() const			{ return "AnimChooser"; }
	static std::string static_group_type()	{ return "AnimChooser"; }

	std::string own_type() const			{ return "AnimChooser"; }
	static std::string static_own_type()	{ return "AnimChooser"; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ICmpAnimChooser> ICmpAnimChooser_Ptr;
typedef shared_ptr<const ICmpAnimChooser> ICmpAnimChooser_CPtr;

}

#endif
