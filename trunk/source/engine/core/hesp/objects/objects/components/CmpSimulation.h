/***
 * hesperus: CmpSimulation.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMPSIMULATION
#define H_HESP_CMPSIMULATION

#include <hesp/level/physics/PhysicsSystem.h>
#include "ICmpSimulation.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
class NormalPhysicsObject;

class CmpSimulation : public ICmpSimulation
{
	//#################### NESTED CLASSES ####################
private:
	struct InitialData
	{
		std::string boundsGroup;
		double dampingFactor;
		std::string posture;
		double inverseMass;
		PhysicsMaterial material;
		Vector3d position;
		Vector3d velocity;

		InitialData(const std::string& boundsGroup_, double dampingFactor_, const std::string& posture_, double inverseMass_,
					PhysicsMaterial material_, const Vector3d& position_, const Vector3d& velocity_)
		:	boundsGroup(boundsGroup_), dampingFactor(dampingFactor_), posture(posture_), inverseMass(inverseMass_),
			material(material_), position(position_), velocity(velocity_)
		{}
	};

	//#################### PRIVATE VARIABLES ####################
private:
	double m_gravityStrength;
	shared_ptr<InitialData> m_initialData;
	NormalPhysicsObject *m_physicsObject;
	PhysicsObjectHandle m_physicsObjectHandle;

	//#################### CONSTRUCTORS ####################
public:
	explicit CmpSimulation(const std::string& boundsGroup, double dampingFactor, double gravityStrength, const std::string& posture, double inverseMass, PhysicsMaterial material, const Vector3d& position, const Vector3d& velocity);

	//#################### STATIC FACTORY METHODS ####################
public:
	static IObjectComponent_Ptr load(const Properties& properties);

	//#################### PUBLIC METHODS ####################
public:
	const std::string& bounds_group() const;
	const Vector3d& position() const;
	const std::string& posture() const;
	Properties save() const;
	void set_object_manager(ObjectManager *objectManager);
	void set_position(const Vector3d& position);
	void set_posture(const std::string& posture);
	void set_velocity(const Vector3d& velocity);
	const Vector3d& velocity() const;
};

}

#endif
