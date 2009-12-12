/***
 * hesperus: WeightForceGenerator.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "WeightForceGenerator.h"

#include <hesp/physics/PhysicsObject.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
WeightForceGenerator::WeightForceGenerator(double gravityStrength)
:	m_gravityStrength(gravityStrength)
{}

//#################### PUBLIC METHODS ####################
void WeightForceGenerator::update_force(PhysicsObject& object) const
{
	double invMass = object.inverse_mass();
	if(invMass == 0) return;

	double mass = 1/invMass;
	object.apply_force(Vector3d(0, 0, -mass*m_gravityStrength));
}

}
