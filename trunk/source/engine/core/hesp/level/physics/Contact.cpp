/***
 * hesperus: Contact.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Contact.h"

#include "PhysicsObject.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
Contact::Contact(const Vector3d& relativePointA, const Vector3d& relativePointB, const Vector3d& normal, double time,
				 PhysicsObject& objectA, int mapIndexA, const boost::optional<PhysicsObject&>& objectB, const boost::optional<int>& mapIndexB)
:	m_relativePointA(relativePointA), m_relativePointB(relativePointB), m_normal(normal), m_time(time),
	m_objectA(objectA), m_mapIndexA(mapIndexA), m_objectB(objectB), m_mapIndexB(mapIndexB)
{}

//#################### PUBLIC METHODS ####################
int Contact::map_indexA() const									{ return m_mapIndexA; }
const boost::optional<int>& Contact::map_indexB() const			{ return m_mapIndexB; }
const Vector3d& Contact::normal() const							{ return m_normal; }
PhysicsObject& Contact::objectA() const							{ return m_objectA; }
const boost::optional<PhysicsObject&>& Contact::objectB() const	{ return m_objectB; }

double Contact::penetration_depth() const
{
	return (pointB() - pointA()).dot(m_normal);
}

Vector3d Contact::pointA() const
{
	return m_relativePointA + m_objectA.position();
}

Vector3d Contact::pointB() const
{
	if(m_objectB) return m_relativePointB + m_objectB->position();
	else return m_relativePointB;
}

const Vector3d& Contact::relative_pointA() const				{ return m_relativePointA; }
const Vector3d& Contact::relative_pointB() const				{ return m_relativePointB; }
double Contact::time() const									{ return m_time; }

}
