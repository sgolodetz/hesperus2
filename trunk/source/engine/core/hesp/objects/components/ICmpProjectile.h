/***
 * hesperus: ICmpProjectile.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICMPPROJECTILE
#define H_HESP_ICMPPROJECTILE

#include <hesp/objects/base/ObjectComponent.h>

namespace hesp {

class ICmpProjectile : public ObjectComponent
{
	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	/**
	Returns the amount of damage done by the projectile.
	*/
	virtual int damage_amount() const = 0;

	/**
	Returns the ID of the character which fired the projectile.
	*/
	virtual const ObjectID& firer() const = 0;

	//#################### PUBLIC METHODS ####################
public:
	std::string group_type() const			{ return "Projectile"; }
	static std::string static_group_type()	{ return "Projectile"; }

	std::string own_type() const			{ return "Projectile"; }
	static std::string static_own_type()	{ return "Projectile"; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ICmpProjectile> ICmpProjectile_Ptr;
typedef shared_ptr<const ICmpProjectile> ICmpProjectile_CPtr;

}

#endif
