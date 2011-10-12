/***
 * hesperus: ICmpSimulation.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICMPSIMULATION
#define H_HESP_ICMPSIMULATION

#include "ICmpPosition.h"

namespace hesp {

class ICmpSimulation : public ICmpPosition
{
	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual const std::string& bounds_group() const = 0;
	virtual const std::string& posture() const = 0;
	virtual void set_posture(const std::string& posture) = 0;
	virtual void set_velocity(const Vector3d& velocity) = 0;
	virtual const Vector3d& velocity() const = 0;

	//#################### PUBLIC METHODS ####################
public:
	std::string own_type() const			{ return "Simulation"; }
	static std::string static_own_type()	{ return "Simulation"; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ICmpSimulation> ICmpSimulation_Ptr;
typedef shared_ptr<const ICmpSimulation> ICmpSimulation_CPtr;

}

#endif
