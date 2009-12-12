/***
 * hesperus: ICmpBipedAnimChooser.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICMPBIPEDANIMCHOOSER
#define H_HESP_ICMPBIPEDANIMCHOOSER

#include "ICmpAnimChooser.h"

namespace hesp {

class ICmpBipedAnimChooser : public ICmpAnimChooser
{
	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void set_run_flag() = 0;
	virtual void set_walk_flag() = 0;

	//#################### PUBLIC METHODS ####################
public:
	std::string own_type() const			{ return "BipedAnimChooser"; }
	static std::string static_own_type()	{ return "BipedAnimChooser"; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ICmpBipedAnimChooser> ICmpBipedAnimChooser_Ptr;
typedef shared_ptr<const ICmpBipedAnimChooser> ICmpBipedAnimChooser_CPtr;

}

#endif
