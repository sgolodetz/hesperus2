/***
 * hesperus: Contact.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CONTACT
#define H_HESP_CONTACT

#include <boost/optional.hpp>

#include <hesp/math/vectors/Vector3.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
class PhysicsObject;

class Contact
{
	//#################### PRIVATE VARIABLES ####################
private:
	int m_mapIndexA;							// the map index of object A
	boost::optional<int> m_mapIndexB;			// the map index of object B (if any)
	Vector3d m_normal;							// the normal at the contact point
	PhysicsObject& m_objectA;					// the first object involved in the collision
	boost::optional<PhysicsObject&> m_objectB;	// the second object involved in the collision (if any)
	Vector3d m_relativePointA;					// a point on the contact plane associated with object A
												// (relative to A)
	Vector3d m_relativePointB;					// a point on the contact plane associated with object B
												// (relative to B, unless B is the world, in which case it's absolute)
	double m_time;								// a number in [0,1] indicating when in the frame the contact occurred

	//#################### CONSTRUCTORS ####################
public:
	Contact(const Vector3d& relativePointA, const Vector3d& relativePointB, const Vector3d& normal, double time, PhysicsObject& objectA, int mapIndexA, const boost::optional<PhysicsObject&>& objectB = boost::none, const boost::optional<int>& mapIndexB = boost::none);

	//#################### PUBLIC METHODS ####################
public:
	int map_indexA() const;
	const boost::optional<int>& map_indexB() const;
	const Vector3d& normal() const;
	PhysicsObject& objectA() const;
	const boost::optional<PhysicsObject&>& objectB() const;
	double penetration_depth() const;
	Vector3d pointA() const;
	Vector3d pointB() const;
	const Vector3d& relative_pointA() const;
	const Vector3d& relative_pointB() const;
	double time() const;
};

}

#endif
