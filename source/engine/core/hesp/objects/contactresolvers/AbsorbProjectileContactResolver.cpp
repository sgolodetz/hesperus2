/***
 * hesperus: AbsorbProjectileContactResolver.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "AbsorbProjectileContactResolver.h"

#include <hesp/objects/base/ObjectManager.h>
#include <hesp/objects/components/ICmpProjectile.h>
#include <hesp/physics/Contact.h>
#include <hesp/physics/PhysicsObject.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
AbsorbProjectileContactResolver::AbsorbProjectileContactResolver(ObjectManager *objectManager, PhysicsMaterial projectileMaterial)
:	m_objectManager(objectManager), m_projectileMaterial(projectileMaterial)
{}

//#################### DESTRUCTOR ####################
AbsorbProjectileContactResolver::~AbsorbProjectileContactResolver() {}

//#################### PROTECTED METHODS ####################
void AbsorbProjectileContactResolver::resolve_projectile_other(const ObjectID& projectile, const ObjectID& other) const
{
	m_objectManager->queue_for_destruction(projectile);
}

//#################### PRIVATE METHODS ####################
void AbsorbProjectileContactResolver::resolve_object_object(const Contact& contact, const OnionTree_CPtr&) const
{
	// Determine which of the two objects involved in the contact is which by examining the materials involved.
	ObjectID projectile, other;
	if(contact.objectA().material() == m_projectileMaterial)
	{
		projectile = contact.objectA().owner();
		other = contact.objectB()->owner();
	}
	else if(contact.objectB()->material() == m_projectileMaterial)
	{
		projectile = contact.objectB()->owner();
		other = contact.objectA().owner();
	}
	else throw Exception("Inappropriate contact resolver: Neither of the objects involved in the contact had the specified projectile type");

	// Resolve the contact, as long as the non-projectile object is not the one that fired the projectile.
	ICmpProjectile_CPtr cmpProjectile = m_objectManager->get_component(projectile, cmpProjectile);
	if(cmpProjectile && (!cmpProjectile->firer().valid() || cmpProjectile->firer() != other))
	{
		resolve_projectile_other(projectile, other);
	}
}

void AbsorbProjectileContactResolver::resolve_object_world(const Contact& contact) const
{
	m_objectManager->queue_for_destruction(contact.objectA().owner());
}

}
