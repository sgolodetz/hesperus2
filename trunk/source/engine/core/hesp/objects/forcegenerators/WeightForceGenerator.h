/***
 * hesperus: WeightForceGenerator.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_WEIGHTFORCEGENERATOR
#define H_HESP_WEIGHTFORCEGENERATOR

#include <hesp/physics/ForceGenerator.h>

namespace hesp {

class WeightForceGenerator : public ForceGenerator
{
	//#################### PRIVATE VARIABLES ####################
private:
	double m_gravityStrength;

	//#################### CONSTRUCTORS ####################
public:
	explicit WeightForceGenerator(double gravityStrength);

	//#################### PUBLIC METHODS ####################
public:
	void update_force(PhysicsObject& object) const;
};

}

#endif
