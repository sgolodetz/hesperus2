/***
 * hesperus: ICmpActivatable.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICMPACTIVATABLE
#define H_HESP_ICMPACTIVATABLE

#include <hesp/objects/base/ObjectComponent.h>

namespace hesp {

/**
Objects with an ICmpActivatable component can be activated by other objects.
*/
class ICmpActivatable : public ObjectComponent
{
	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void activated_by(const ObjectID& activator) = 0;

	//#################### PUBLIC METHODS ####################
public:
	void check_dependencies() const;

	std::string group_type() const			{ return "Activatable"; }
	static std::string static_group_type()	{ return "Activatable"; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ICmpActivatable> ICmpActivatable_Ptr;
typedef shared_ptr<const ICmpActivatable> ICmpActivatable_CPtr;

}

#endif
