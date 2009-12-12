/***
 * hesperus: PhysicsObject.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_PHYSICSOBJECT
#define H_HESP_PHYSICSOBJECT

#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <hesp/math/vectors/Vector3.h>
#include <hesp/objects/base/ObjectID.h>
#include "PhysicsMaterial.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<const class Bounds> Bounds_CPtr;
typedef shared_ptr<const class BoundsManager> BoundsManager_CPtr;

class PhysicsObject
{
	//#################### FRIENDS ####################
	friend class BroadPhaseCollisionDetector;
	friend class NarrowPhaseCollisionDetector;
	friend class PhysicsSystem;

	//#################### PRIVATE VARIABLES ####################
private:
	Vector3d m_accumulatedForce;
	int m_id;
	double m_inverseMass;
	PhysicsMaterial m_material;
	ObjectID m_owner;
	Vector3d m_position;
	boost::optional<Vector3d> m_previousPosition;
	bool m_sleeping;
	Vector3d m_velocity;

	//#################### CONSTRUCTORS ####################
public:
	PhysicsObject(double inverseMass, PhysicsMaterial material, const ObjectID& owner, const Vector3d& position, const Vector3d& velocity);

	//#################### DESTRUCTOR ####################
public:
	virtual ~PhysicsObject();

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual Bounds_CPtr bounds(const BoundsManager_CPtr& boundsManager) const = 0;
	virtual void update(int milliseconds) = 0;

	//#################### PUBLIC METHODS ####################
public:
	void apply_force(const Vector3d& force);
	double inverse_mass() const;
	PhysicsMaterial material() const;
	const ObjectID& owner() const;
	const Vector3d& position() const;
	void set_position(Vector3d position);
	void set_velocity(const Vector3d& velocity);
	const Vector3d& velocity() const;

	//#################### PROTECTED METHODS ####################
protected:
	const Vector3d& accumulated_force() const;

	//#################### PRIVATE METHODS ####################
private:
	void clear_accumulated_force();
	int id() const;
	bool is_sleeping() const;
	const boost::optional<Vector3d>& previous_position() const;
	void set_id(int id);
	void set_sleeping(bool sleeping);
};

}

#endif
