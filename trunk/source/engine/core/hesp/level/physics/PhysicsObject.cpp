/***
 * hesperus: PhysicsObject.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "PhysicsObject.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
PhysicsObject::PhysicsObject(double inverseMass, PhysicsMaterial material, const ObjectID& owner, const Vector3d& position,
							 const Vector3d& velocity)
:	m_id(-1), m_sleeping(false), m_inverseMass(inverseMass), m_material(material), m_owner(owner), m_position(position), m_velocity(velocity)
{}

//#################### DESTRUCTOR ####################
PhysicsObject::~PhysicsObject() {}

//#################### PUBLIC METHODS ####################
void PhysicsObject::apply_force(const Vector3d& force)						{ m_accumulatedForce += force; }
double PhysicsObject::inverse_mass() const									{ return m_inverseMass; }
PhysicsMaterial PhysicsObject::material() const								{ return m_material; }
const ObjectID& PhysicsObject::owner() const								{ return m_owner; }
const Vector3d& PhysicsObject::position() const								{ return m_position; }

void PhysicsObject::set_position(Vector3d position)
{
	m_previousPosition = m_position;
	m_position = position;
}

void PhysicsObject::set_velocity(const Vector3d& velocity)					{ m_velocity = velocity; }
const Vector3d& PhysicsObject::velocity() const								{ return m_velocity; }

//#################### PROTECTED METHODS ####################
const Vector3d& PhysicsObject::accumulated_force() const					{ return m_accumulatedForce; }

//#################### PRIVATE METHODS ####################
void PhysicsObject::clear_accumulated_force()								{ m_accumulatedForce = Vector3d(0,0,0); }
int PhysicsObject::id() const												{ return m_id; }
bool PhysicsObject::is_sleeping() const										{ return m_sleeping; }
const boost::optional<Vector3d>& PhysicsObject::previous_position() const	{ return m_previousPosition; }
void PhysicsObject::set_id(int id)											{ m_id = id; }
void PhysicsObject::set_sleeping(bool sleeping)								{ m_sleeping = sleeping; }

}
