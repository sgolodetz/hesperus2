/***
 * hesperus: SpringForceGenerator.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "SpringForceGenerator.h"

#include <cmath>

#include <hesp/math/Constants.h>
#include <hesp/physics/PhysicsObject.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
SpringForceGenerator::SpringForceGenerator(double naturalLength, double springConstant,
										   const PhysicsObject_CPtr& otherEnd, PhysicsObjectID otherEndID)
:	m_naturalLength(naturalLength), m_springConstant(springConstant), m_otherEnd(otherEnd), m_otherEndID(otherEndID)
{}

//#################### PUBLIC METHODS ####################
std::vector<PhysicsObjectID> SpringForceGenerator::referenced_objects() const
{
	std::vector<PhysicsObjectID> ret;
	ret.push_back(m_otherEndID);
	return ret;
}

void SpringForceGenerator::update_force(PhysicsObject& object) const
{
	Vector3d force = m_otherEnd->position() - object.position();
	double length = force.length();
	if(fabs(length) < SMALL_EPSILON) return;

	double extension = length - m_naturalLength;
	force *= m_springConstant * extension / length;

	object.apply_force(force);
}

}
