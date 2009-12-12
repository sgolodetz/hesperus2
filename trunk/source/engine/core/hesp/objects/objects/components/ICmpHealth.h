/***
 * hesperus: ICmpHealth.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICMPHEALTH
#define H_HESP_ICMPHEALTH

#include <hesp/level/objects/base/ObjectComponent.h>

namespace hesp {

class ICmpHealth : public ObjectComponent
{
	//#################### ENUMERATIONS ####################
public:
	enum HealthStatus
	{
		ALIVE,
		DEAD,
		DYING
	};

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void damage(int amount) = 0;
	virtual int health() const = 0;
	virtual void kill() = 0;
	virtual int max_health() const = 0;
	virtual HealthStatus status() const = 0;

	//#################### PUBLIC METHODS ####################
public:
	std::string group_type() const			{ return "Health"; }
	static std::string static_group_type()	{ return "Health"; }

	std::string own_type() const			{ return "Health"; }
	static std::string static_own_type()	{ return "Health"; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ICmpHealth> ICmpHealth_Ptr;
typedef shared_ptr<const ICmpHealth> ICmpHealth_CPtr;

}

#endif
