/***
 * hesperus: NormalPhysicsObject.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "NormalPhysicsObject.h"

#include <cmath>

#include <hesp/bounds/BoundsManager.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
NormalPhysicsObject::NormalPhysicsObject(const std::string& boundsGroup, double dampingFactor, double inverseMass, PhysicsMaterial material,
										 const ObjectID& owner, const Vector3d& position, const std::string& posture, const Vector3d& velocity)
:	PhysicsObject(inverseMass, material, owner, position, velocity), m_boundsGroup(boundsGroup), m_posture(posture), m_dampingFactor(dampingFactor)
{}

//#################### PUBLIC METHODS ####################
Bounds_CPtr NormalPhysicsObject::bounds(const BoundsManager_CPtr& boundsManager) const
{
	return boundsManager->bounds(m_boundsGroup, m_posture);
}

const std::string& NormalPhysicsObject::bounds_group() const
{
	return m_boundsGroup;
}

double NormalPhysicsObject::damping_factor() const
{
	return m_dampingFactor;
}

const std::string& NormalPhysicsObject::posture() const
{
	return m_posture;
}

void NormalPhysicsObject::set_posture(const std::string& posture)
{
	m_posture = posture;
}

void NormalPhysicsObject::update(int milliseconds)
{
	double t = milliseconds / 1000.0;
	Vector3d acceleration = inverse_mass() * accumulated_force();
	set_position(position() + velocity() * t + 0.5 * acceleration * t * t);
	set_velocity(velocity() + acceleration * t);

	// Damp the velocity where necessary.
	set_velocity(velocity() * m_dampingFactor);
}

}
