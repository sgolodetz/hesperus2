/***
 * hesperus: ProjectileDamageContactResolver.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ProjectileDamageContactResolver.h"

#include <hesp/objects/components/ICmpHealth.h>
#include <hesp/objects/components/ICmpProjectile.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
ProjectileDamageContactResolver::ProjectileDamageContactResolver(ObjectManager *objectManager, PhysicsMaterial projectileMaterial)
:	AbsorbProjectileContactResolver(objectManager, projectileMaterial)
{}

//#################### PRIVATE METHODS ####################
void ProjectileDamageContactResolver::resolve_projectile_other(const ObjectID& projectile, const ObjectID& other) const
{
	// Damage the non-projectile object.
	ICmpHealth_Ptr cmpOtherHealth = m_objectManager->get_component(other, cmpOtherHealth);
	if(!cmpOtherHealth) throw Exception("The non-projectile object involved in the contact has no health component");
	ICmpProjectile_CPtr cmpProjectile = m_objectManager->get_component(projectile, cmpProjectile);
	int amount = cmpProjectile->damage_amount();
	cmpOtherHealth->damage(amount);

	// Absorb the projectile.
	AbsorbProjectileContactResolver::resolve_projectile_other(projectile, other);
}

}
