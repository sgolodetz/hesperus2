/***
 * hesperus: AbsorbProjectileContactResolver.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ABSORBPROJECTILECONTACTRESOLVER
#define H_HESP_ABSORBPROJECTILECONTACTRESOLVER

#include <hesp/level/physics/ContactResolver.h>
#include <hesp/level/physics/PhysicsMaterial.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
class ObjectID;
class ObjectManager;

class AbsorbProjectileContactResolver : public ContactResolver
{
	//#################### PRIVATE VARIABLES ####################
private:
	PhysicsMaterial m_projectileMaterial;

	//#################### PROTECTED VARIABLES ####################
protected:
	ObjectManager *m_objectManager;

	//#################### CONSTRUCTORS ####################
public:
	AbsorbProjectileContactResolver(ObjectManager *objectManager, PhysicsMaterial projectileMaterial);

	//#################### DESTRUCTOR ####################
public:
	virtual ~AbsorbProjectileContactResolver();

	//#################### PROTECTED METHODS ####################
protected:
	virtual void resolve_projectile_other(const ObjectID& projectile, const ObjectID& other) const;

	//#################### PRIVATE METHODS ####################
private:
	void resolve_object_object(const Contact& contact, const OnionTree_CPtr&) const;
	void resolve_object_world(const Contact& contact) const;
};

}

#endif
