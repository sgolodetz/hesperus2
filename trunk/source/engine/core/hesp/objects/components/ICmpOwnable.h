/***
 * hesperus: ICmpOwnable.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICMPOWNABLE
#define H_HESP_ICMPOWNABLE

#include <hesp/objects/base/ObjectComponent.h>

namespace hesp {

/**
Objects with an ICmpOwnable component can be owned (and held) by other objects.
*/
class ICmpOwnable : public ObjectComponent
{
	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual const std::string& anim_extension() const = 0;
	virtual const std::string& attach_point() const = 0;
	virtual void clear_owner() = 0;
	virtual const ObjectID& owner() const = 0;
	virtual void set_owner(const ObjectID& owner) = 0;

	//#################### PUBLIC METHODS ####################
public:
	std::string group_type() const			{ return "Ownable"; }
	static std::string static_group_type()	{ return "Ownable"; }

	std::string own_type() const			{ return "Ownable"; }
	static std::string static_own_type()	{ return "Ownable"; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ICmpOwnable> ICmpOwnable_Ptr;
typedef shared_ptr<const ICmpOwnable> ICmpOwnable_CPtr;

}

#endif
