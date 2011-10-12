/***
 * hesperus: BounceContactResolver.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "BounceContactResolver.h"

#include <hesp/math/Constants.h>
#include <hesp/physics/Contact.h>
#include <hesp/physics/PhysicsObject.h>
#include <hesp/trees/OnionUtil.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
BounceContactResolver::BounceContactResolver(double restitutionCoefficient)
:	m_restitutionCoefficient(restitutionCoefficient)
{}

//#################### PRIVATE METHODS ####################
void BounceContactResolver::resolve_object_object(const Contact& contact, const OnionTree_CPtr& tree) const
{
	// Step 1:	Calculate what fractions of the resolution will apply to each of the two objects involved.
	PhysicsObject& objectA = contact.objectA();
	PhysicsObject& objectB = *contact.objectB();
	double invMassA = objectA.inverse_mass();
	double invMassB = objectB.inverse_mass();
	double totalInvMass = invMassA + invMassB;

	// We can't resolve a collision between two objects with infinite mass, since we can't move either of them.
	if(totalInvMass < EPSILON) return;

	double fractionA = invMassA / totalInvMass;
	double fractionB = invMassB / totalInvMass;

	// Step 2:	Resolve interpenetration. For object-object contacts, this involves moving the
	//			objects apart along the contact normal in proportion to their inverse masses.
	double penetrationDepth = contact.penetration_depth();
	double depthA = penetrationDepth * fractionA;
	double depthB = penetrationDepth * fractionB;

	// Update the positions of the objects to resolve the interpenetration, without letting either object
	// end up embedded in the world.
	Vector3d newPosA = objectA.position() + contact.normal() * depthA;
	Vector3d newPosB = objectB.position() - contact.normal() * depthB;

	Vector3d fixedPosA = newPosA;
	if(contact.map_indexA() != -1)
	{
		OnionUtil::Transition transitionA = OnionUtil::find_first_transition(contact.map_indexA(), objectA.position(), fixedPosA, tree);
		switch(transitionA.classifier)
		{
		case OnionUtil::RAY_SOLID:
			fixedPosA = objectA.position();
			break;
		case OnionUtil::RAY_TRANSITION_ES:
			fixedPosA = *transitionA.location;
			break;
		default:
			break;
		}
	}

	Vector3d fixedPosB = newPosB + (fixedPosA - newPosA);
	if(*contact.map_indexB() != -1)
	{
		OnionUtil::Transition transitionB = OnionUtil::find_first_transition(*contact.map_indexB(), objectB.position(), fixedPosB, tree);
		switch(transitionB.classifier)
		{
		case OnionUtil::RAY_SOLID:
			fixedPosB = objectB.position();
			break;
		case OnionUtil::RAY_TRANSITION_ES:
			fixedPosB = *transitionB.location;
			break;
		default:
			break;
		}
	}

	objectA.set_position(fixedPosA);
	objectB.set_position(fixedPosB);

	// Step 3:	Determine the new velocities of the objects.

	// Calculate the old separating velocity (in the normal direction).
	double oldSepVelocity = (objectA.velocity() - objectB.velocity()).dot(contact.normal());

	// If the contact is either separating or stationary, there's nothing to do.
	if(oldSepVelocity >= 0) return;

	// Calculate the new separating velocity (in the normal direction).
	double newSepVelocity = -m_restitutionCoefficient * oldSepVelocity;

	// Update the velocities of the objects.
	double deltaVel = newSepVelocity - oldSepVelocity;
	Vector3d deltaVelA = deltaVel * fractionA * contact.normal();
	Vector3d deltaVelB = deltaVel * fractionB * -contact.normal();
	objectA.set_velocity(objectA.velocity() + deltaVelA);
	objectB.set_velocity(objectB.velocity() + deltaVelB);
}

void BounceContactResolver::resolve_object_world(const Contact& contact) const
{
	// Step 1:	Resolve interpenetration. For object-world contacts, this involves moving the object
	//			back to the transition location, which is stored as point B in the contact structure.
	PhysicsObject& objectA = contact.objectA();
	objectA.set_position(contact.pointB());

	// Step 2:	Determine the new velocity of the object.

	// Calculate the old separating velocity (in the normal direction).
	double oldSepVelocity = objectA.velocity().dot(contact.normal());

	// If the contact is either separating or stationary, there's nothing to do.
	if(oldSepVelocity >= 0) return;

	// Calculate the new separating velocity (in the normal direction).
	double newSepVelocity = -m_restitutionCoefficient * oldSepVelocity;

	// Update the velocity of the object.
	Vector3d deltaVelA = (newSepVelocity - oldSepVelocity) * contact.normal();
	objectA.set_velocity(objectA.velocity() + deltaVelA);
}

}
