/***
 * hesperus: NormalPhysicsObject.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_NORMALPHYSICSOBJECT
#define H_HESP_NORMALPHYSICSOBJECT

#include "PhysicsObject.h"

namespace hesp {

class NormalPhysicsObject : public PhysicsObject
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::string m_boundsGroup;
	std::string m_posture;

	double m_dampingFactor;

	//#################### CONSTRUCTORS ####################
public:
	NormalPhysicsObject(const std::string& boundsGroup, double dampingFactor, double inverseMass, PhysicsMaterial material, const ObjectID& owner, const Vector3d& position, const std::string& posture, const Vector3d& velocity = Vector3d(0,0,0));

	//#################### PUBLIC METHODS ####################
public:
	Bounds_CPtr bounds(const BoundsManager_CPtr& boundsManager) const;
	const std::string& bounds_group() const;
	double damping_factor() const;
	const std::string& posture() const;
	void set_posture(const std::string& posture);
	void update(int milliseconds);
};

//#################### TYPEDEFS ####################
typedef shared_ptr<NormalPhysicsObject> NormalPhysicsObject_Ptr;
typedef shared_ptr<const NormalPhysicsObject> NormalPhysicsObject_CPtr;

}

#endif
