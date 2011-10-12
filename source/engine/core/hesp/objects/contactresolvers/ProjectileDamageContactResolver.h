/***
 * hesperus: ProjectileDamageContactResolver.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_PROJECTILEDAMAGECONTACTRESOLVER
#define H_HESP_PROJECTILEDAMAGECONTACTRESOLVER

#include "AbsorbProjectileContactResolver.h"

namespace hesp {

class ProjectileDamageContactResolver : public AbsorbProjectileContactResolver
{
	//#################### CONSTRUCTORS ####################
public:
	ProjectileDamageContactResolver(ObjectManager *objectManager, PhysicsMaterial projectileMaterial);

	//#################### PRIVATE METHODS ####################
private:
	void resolve_projectile_other(const ObjectID& projectile, const ObjectID& other) const;
};

}

#endif
