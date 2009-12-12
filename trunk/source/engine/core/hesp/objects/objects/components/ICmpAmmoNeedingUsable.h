/***
 * hesperus: ICmpAmmoNeedingUsable.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICMPAMMONEEDINGUSABLE
#define H_HESP_ICMPAMMONEEDINGUSABLE

#include "ICmpUsable.h"

namespace hesp {

/**
Objects with an ICmpAmmoNeedingUsable component can only be used while sufficient ammo is available.
*/
class ICmpAmmoNeedingUsable : public virtual ICmpUsable
{
	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual const std::string& ammo_type() const = 0;

	//#################### PUBLIC METHODS ####################
public:
	std::string own_type() const			{ return "AmmoNeedingUsable"; }
	static std::string static_own_type()	{ return "AmmoNeedingUsable"; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ICmpAmmoNeedingUsable> ICmpAmmoNeedingUsable_Ptr;
typedef shared_ptr<const ICmpAmmoNeedingUsable> ICmpAmmoNeedingUsable_CPtr;

}

#endif
