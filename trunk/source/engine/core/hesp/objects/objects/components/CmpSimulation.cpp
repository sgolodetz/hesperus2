/***
 * hesperus: CmpSimulation.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpSimulation.h"

#include <hesp/level/objects/forcegenerators/WeightForceGenerator.h>
#include <hesp/level/physics/NormalPhysicsObject.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpSimulation::CmpSimulation(const std::string& boundsGroup, double dampingFactor, double gravityStrength, const std::string& posture, double inverseMass,
							 PhysicsMaterial material, const Vector3d& position, const Vector3d& velocity)
:	m_initialData(new InitialData(boundsGroup, dampingFactor, posture, inverseMass, material, position, velocity)), m_gravityStrength(gravityStrength)
{}

//#################### STATIC FACTORY METHODS ####################
IObjectComponent_Ptr CmpSimulation::load(const Properties& properties)
{
	return IObjectComponent_Ptr(new CmpSimulation(
		properties.get<std::string>("BoundsGroup"),
		properties.get<double>("DampingFactor"),
		properties.get<double>("GravityStrength"),
		properties.get<std::string>("Posture"),
		properties.get<double>("InverseMass"),
		properties.get<PhysicsMaterial>("Material"),
		properties.get<Vector3d>("Position"),
		properties.get<Vector3d>("Velocity")
	));
}

//#################### PUBLIC METHODS ####################
const std::string& CmpSimulation::bounds_group() const
{
	return m_physicsObject->bounds_group();
}

const Vector3d& CmpSimulation::position() const
{
	return m_physicsObject->position();
}

const std::string& CmpSimulation::posture() const
{
	return m_physicsObject->posture();
}

Properties CmpSimulation::save() const
{
	Properties properties;
	properties.set("BoundsGroup", bounds_group());
	properties.set("DampingFactor", m_physicsObject->damping_factor());
	properties.set("GravityStrength", m_gravityStrength);
	properties.set("InverseMass", m_physicsObject->inverse_mass());
	properties.set("Material", m_physicsObject->material());
	properties.set("Position", position());
	properties.set("Posture", posture());
	properties.set("Velocity", velocity());
	return properties;
}

void CmpSimulation::set_object_manager(ObjectManager *objectManager)
{
	IObjectComponent::set_object_manager(objectManager);

	// Register a new physics object with the specified initial data. Note that this
	// can only be done at this point, because we need a handle to the physics system
	// and we don't have one until the object manager has been set.
	const PhysicsSystem_Ptr& physicsSystem = objectManager->physics_system();
	assert(m_objectID.valid());
	m_physicsObject = new NormalPhysicsObject(
		m_initialData->boundsGroup, m_initialData->dampingFactor, m_initialData->inverseMass,
		m_initialData->material, m_objectID, m_initialData->position, m_initialData->posture, m_initialData->velocity
	);
	m_physicsObjectHandle = physicsSystem->register_object(PhysicsObject_Ptr(m_physicsObject));
	m_initialData.reset();

	// If the gravity strength is non-zero, add a weight force generator.
	if(m_gravityStrength > 0)
	{
		physicsSystem->set_force_generator(m_physicsObjectHandle, "Weight", ForceGenerator_CPtr(new WeightForceGenerator(m_gravityStrength)));
	}
}

void CmpSimulation::set_position(const Vector3d& position)
{
	m_physicsObject->set_position(position);
}

void CmpSimulation::set_posture(const std::string& posture)
{
	m_physicsObject->set_posture(posture);
}

void CmpSimulation::set_velocity(const Vector3d& velocity)
{
	m_physicsObject->set_velocity(velocity);
}

const Vector3d& CmpSimulation::velocity() const
{
	return m_physicsObject->velocity();
}

}
