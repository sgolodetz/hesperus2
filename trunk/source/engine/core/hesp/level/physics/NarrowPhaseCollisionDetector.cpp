/***
 * hesperus: NarrowPhaseCollisionDetector.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "NarrowPhaseCollisionDetector.h"

#include <cmath>

#include <boost/tuple/tuple.hpp>

#include <hesp/exceptions/Exception.h>
#include <hesp/level/bounds/BoundsManager.h>
#include <hesp/level/trees/OnionUtil.h>
#include <hesp/math/Constants.h>
#include <hesp/math/geom/Plane.h>
#include "BoundsSupportMapping.h"
#include "MinkDiffSupportMapping.h"
#include "NormalPhysicsObject.h"
#include "SegmentSupportMapping.h"
#include "SweptSupportMapping.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
NarrowPhaseCollisionDetector::NarrowPhaseCollisionDetector(const BoundsManager_CPtr& boundsManager,
														   const OnionTree_CPtr& tree)
:	m_boundsManager(boundsManager), m_tree(tree)
{}

//#################### PUBLIC METHODS ####################
boost::optional<Contact>
NarrowPhaseCollisionDetector::object_vs_object(PhysicsObject& objectA, PhysicsObject& objectB) const
{
	/*
	The algorithm used is known as Minkowski Portal Refinement (MPR), or XenoCollide,
	which is described in an article in Game Programming Gems 7 (starting on p.165).

	The conversion to a swept collision test is based on a post by Gary Snethen on
	the XenoCollide forums.
	*/

	SupportMapping_CPtr mapping, mappingA, mappingS;
	Vector3d interiorPoint;
	Vector3d relativeMovement;
	construct_support_mappings(objectA, objectB, mapping, mappingA, mappingS, interiorPoint, relativeMovement);
	boost::optional<Contact> contact = xeno_collide(objectA, objectB, mapping, mappingA, mappingS,
													interiorPoint, relativeMovement);
	return convert_to_world_contact(contact);
}

boost::optional<Contact>
NarrowPhaseCollisionDetector::object_vs_world(NormalPhysicsObject& object) const
{
	const Vector3d& previousPos = object.previous_position() ? *object.previous_position() : object.position();
	const Vector3d& pos = object.position();

	int mapIndex = m_boundsManager->lookup_bounds_index(object.bounds_group(), object.posture());
	OnionUtil::Transition transition = OnionUtil::find_first_transition(mapIndex, previousPos, pos, m_tree);

	switch(transition.classifier)
	{
		case OnionUtil::RAY_EMPTY:
		{
			// The object's movement lies entirely in empty space, so there's no collision.
			return boost::none;
		}
		case OnionUtil::RAY_SOLID:
		{
			Vector3d dir = pos - previousPos;
			if(dir.length_squared() >= EPSILON*EPSILON)
			{
				dir.normalize();
				transition = OnionUtil::find_first_transition(mapIndex, previousPos - dir * 0.1, pos, m_tree);

				// If we still haven't found a transition, there's probably no collision to handle.
				// If we have, fall through to the next case.
				if(transition.classifier != OnionUtil::RAY_TRANSITION_ES) return boost::none;
			}
			else return boost::none;
		}
		case OnionUtil::RAY_TRANSITION_ES:
		{
			const Vector3d& contactPoint = *transition.location;
			const Vector3d& contactNormal = transition.plane->normal();

			// Calculate the intersection time (as a number in [0,1]).
			double contactDistSquared = (contactPoint - previousPos).length_squared();
			double moveDistSquared = (pos - previousPos).length_squared();
			assert(moveDistSquared > 0);
			double time = sqrt(contactDistSquared / moveDistSquared);

			return Contact(Vector3d(0,0,0), contactPoint, contactNormal, time, object, mapIndex);
		}
		case OnionUtil::RAY_TRANSITION_SE:
		{
			// This should never happen (since objects can't move into walls), but better let the object back into
			// the world if it does happen.
			return boost::none;
		}
		default:
		{
			throw Exception("This will never happen as the case statement is exhaustive");
		}
	}
}

//#################### PRIVATE METHODS ####################
void NarrowPhaseCollisionDetector::construct_support_mappings(const PhysicsObject& objectA, const PhysicsObject& objectB,
															  SupportMapping_CPtr& mapping, SupportMapping_CPtr& mappingA,
															  SupportMapping_CPtr& mappingS, Vector3d& interiorPoint,
															  Vector3d& relativeMovement) const
{
	// Construct the support mapping in the reference frame of A. (In that frame, A is centred at the origin.)
	// We'll be colliding a relative, swept version of B (called S) against a stationary A and then transforming
	// the result back into world space.

	Vector3d aPos0 = objectA.previous_position() ? *objectA.previous_position() : objectA.position();
	Vector3d aPos1 = objectA.position();
	Vector3d bPos0 = objectB.previous_position() ? *objectB.previous_position() : objectB.position();
	Vector3d bPos1 = objectB.position();

	Vector3d bRelPos0 = bPos0 - aPos0;
	Vector3d bRelPos1 = bPos1 - aPos1;
	relativeMovement = bRelPos1 - bRelPos0;

	// Construct the support mapping for the stationary A.
	Bounds_CPtr aBounds = objectA.bounds(m_boundsManager);
	mappingA.reset(new BoundsSupportMapping(aBounds));

	// Construct the support mapping for S (the relative, swept B).
	Bounds_CPtr bBounds = objectB.bounds(m_boundsManager);
	SupportMapping_CPtr mappingB(new BoundsSupportMapping(bBounds));
	SupportMapping_CPtr mappingL(new SegmentSupportMapping(bRelPos0, bRelPos1));
	mappingS.reset(new SweptSupportMapping(mappingB, mappingL));

	// Construct the support mapping for their Minkowski difference (S - A).
	mapping.reset(new MinkDiffSupportMapping(mappingS, mappingA));

	// Find a point interior to S - A. Note that the midpoint of B's relative movement
	// vector will do because it's definitely inside S (which is the relative sweep of
	// B as it moves), whilst A is centred at the origin and symmetrical about it, so
	// subtracting A won't move the overall shape.
	interiorPoint = (bRelPos0 + bRelPos1) / 2;

	// Ensure that the interior point isn't too near the origin. If it is, any point
	// right next to the origin can be substituted instead.
	if(interiorPoint.length_squared() < EPSILON*EPSILON) interiorPoint = Vector3d(EPSILON, 0, 0);
}

boost::optional<Contact>
NarrowPhaseCollisionDetector::convert_to_world_contact(const boost::optional<Contact>& relativeContact) const
{
	if(!relativeContact) return boost::none;
	const Contact& rc = *relativeContact;

	// This makes the relative point on B relative to the centre of B rather than to the centre of A.
	return Contact(rc.relative_pointA(),
				   rc.relative_pointB() + rc.objectA().position() - rc.objectB()->position(),
				   rc.normal(), rc.time(), rc.objectA(), rc.map_indexA(), rc.objectB(), rc.map_indexB());
}

Contact NarrowPhaseCollisionDetector::make_contact(const Vector3d& v0, const SupportMapping_CPtr& mappingA,
												   const SupportMapping_CPtr& mappingB, const Vector3d& relativeMovement,
												   PhysicsObject& objectA, PhysicsObject& objectB) const
{
	Vector3d contactNormal = (-v0).normalize();
	Vector3d contactPointA = (*mappingA)(-contactNormal);
	Vector3d contactPointB = (*mappingB)(contactNormal);
	double penetrationDepth = (contactPointB - contactPointA).dot(contactNormal);
	assert(penetrationDepth > 0);
	double contactDistanceMoved = relativeMovement.dot(contactNormal);
	double time = contactDistanceMoved > 0 ? 1 - penetrationDepth/contactDistanceMoved : 0;
	NormalPhysicsObject *nobjectA = dynamic_cast<NormalPhysicsObject*>(&objectA);
	NormalPhysicsObject *nobjectB = dynamic_cast<NormalPhysicsObject*>(&objectB);
	int mapIndexA = nobjectA ? m_boundsManager->lookup_bounds_index(nobjectA->bounds_group(), nobjectA->posture()) : -1;
	int mapIndexB = nobjectB ? m_boundsManager->lookup_bounds_index(nobjectB->bounds_group(), nobjectB->posture()) : -1;
	return Contact(contactPointA, contactPointB, contactNormal, time, objectA, mapIndexA, objectB, mapIndexB);
}

boost::optional<Contact>
NarrowPhaseCollisionDetector::xeno_collide(PhysicsObject& objectA, PhysicsObject& objectB,
										   const SupportMapping_CPtr& mapping, const SupportMapping_CPtr& mappingA,
										   const SupportMapping_CPtr& mappingB, const Vector3d& v0,
										   const Vector3d& relativeMovement) const
try
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Phase 1: Portal Discovery
	//~~~~~~~~~~~~~~~~~~~~~~~~~~

	Vector3d v1, v2, v3;

	// Find the support point in the direction of the origin ray 0 - v0 = -v0.
	Vector3d n = (-v0).normalize();		// note: v0 is guaranteed to be non-zero by construction
	v1 = (*mapping)(n);

	if(n.dot(v1) <= 0)
	{
		// Special Case: The origin doesn't lie on the near side of the support plane containing v1 -> MISS.
		// (Note that the plane equation is given by n.x - n.v1 = 0, so if n.v1 <= 0 then n.x - n.v1 >= 0,
		// i.e. the origin is in front of, or on, a support plane pointing away from the shape.)
		return boost::none;
	}

	// Find the support point along the normal of the plane containing the origin, v0 and v1.
	n = v0.cross(v1);

	if(n.length_squared() < EPSILON*EPSILON)
	{
		// Special Case: v1 lies on the line joining the origin and v0.
		// Since it's a point on the surface of the Minkowski difference shape,
		// we can use it to test directly whether or not the origin is within
		// the shape.
		Vector3d v01 = v1 - v0;
		if(v0.length_squared() < v01.length_squared())
		{
			// v0 is closer to the origin than it is to v1, so (by the geometry of the situation) the origin
			// must lie between v0 and v1 and thus be inside the Minkowski difference shape -> HIT.
			return make_contact(v0, mappingA, mappingB, relativeMovement, objectA, objectB);
		}
		else return boost::none;
	}

	n.normalize();	// note: if we get here, n is non-zero
	v2 = (*mapping)(n);

	if(n.dot(v2) <= 0)
	{
		// Special Case: The origin doesn't lie on the near side of the support plane containing v2 -> MISS.
		return boost::none;
	}

	// Calculate the normal for the plane containing v0, v1 and v2.
	n = (v1 - v0).cross(v2 - v0);

	// We know that v1 != v0 and v2 != v0 by assumption that v0 is an interior point of the shape
	// (note that v1 and v2 are both support points, and thus on the surface by definition). The
	// only remaining way n could be 0 is if v1 - v0 and v2 - v0 were in the same direction. In
	// that case, v2 would lie in the same plane as the origin, v0 and v1, since v1 - v0 does. But
	// v2 is the support point in a direction perpendicular to this plane, so that could only happen
	// if the plane itself was on the surface. But then v0 would be on the surface -> contradiction.
	n.normalize();

	// Ensure the origin is in front of the plane (which has equation n.x - n.v0 = 0)
	// by swapping v1 and v2 if necessary. Note that to plug the origin into the equation
	// we test n.0 - n.v0 = -n.v0 against 0. If it's less than 0, the origin's behind the
	// plane. (This is the same as checking for n.v0 > 0.)
	if(n.dot(v0) > 0)
	{
		std::swap(v1, v2);
		n = -n;
	}

	// Iteratively find the remaining support point we need. Initially we look along the normal of the
	// plane containing triangle (v0, v1, v2).
	const int ITERATION_LIMIT = INT_MAX;	// we limit the amount of work the collision detector is allowed to do
	int iterations = 0;
	while(iterations++ < ITERATION_LIMIT)
	{
		// Find the support point in the direction normal to the existing plane.
		v3 = (*mapping)(n);
		if(n.dot(v3) <= 0)
		{
			// Special Case: The origin doesn't lie on the near side of the support plane containing v3 -> MISS.
			return boost::none;
		}

		// Check the origin against the the two other non-portal planes - if it's not in front of both of them,
		// we need to keep looking for support points to make up our initial portal.

		// Check against the plane containing triangle (v0, v2, v3). This plane has equation
		// n.x - n.v0 = 0, where n = (v2-v0) x (v3 - v0). To test if the origin is behind it,
		// we therefore check -n.v0 < 0, i.e. n.v0 > 0. This is v0 . ((v2 - v0) x (v3 - v0)) > 0.
		// But note the identity a.((b-a) x (c-a)) = a.b x c, so this simplifies to v0.v2 x v3 > 0.
		if(v0.dot(v2.cross(v3)) > 0)
		{
			// The origin is behind the plane containing (v0, v2, v3), so set things up
			// to search for the third support point in front of that plane. In particular,
			// we want to set (v0, v1, v2) := (v0, v3, v2), i.e. the reverse of the plane
			// the origin's behind.
			v1 = v3;
			n = (v1 - v0).cross(v2 - v0).normalize();	// note that our plane is now that of triangle (v0, v1, v2)
			continue;
		}

		// Check against the plane containing triangle (v0, v3, v1).
		if(v0.dot(v3.cross(v1)) > 0)
		{
			// The origin is behind the plane containing (v0, v3, v1), so set things up
			// to search for the third support point in front of that plane. In particular,
			// we want to set (v0, v1, v2) := (v0, v1, v3), i.e. the reverse of the plane
			// the origin's behind.
			v2 = v3;
			n = (v1 - v0).cross(v2 - v0).normalize();	// note that our plane is now that of triangle (v0, v1, v2)
			continue;
		}

		// If we got here, we found a valid portal.
		break;
	}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Phase 2: Portal Refinement
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~

	while(iterations++ < ITERATION_LIMIT)
	{
		// Check whether the origin's behind the portal (v1, v2, v3), which faces towards the outside of the shape.
		// If so, it's inside the shape -> HIT. The plane is n.x - n.v1 = 0, where n = (v2-v1) x (v3-v1). The origin
		// is behind it if -n.v1 < 0, i.e. if n.v1 > 0.
		n = (v2 - v1).cross(v3 - v1).normalize();
		if(n.dot(v1) > 0)
		{
			return make_contact(v0, mappingA, mappingB, relativeMovement, objectA, objectB);
		}

		// If the origin's not behind the portal, find the support plane in the direction of the portal normal.
		// This has equation n.x - n.v4 = 0.
		Vector3d v4 = (*mapping)(n);

		// Check whether the origin's not behind the support plane, which faces towards the outside of the shape.
		// If so, it's outside the shape -> MISS. The origin's not behind the support plane if -n.v4 >= 0, i.e.
		// if n.v4 <= 0.
		if(n.dot(v4) <= 0)
		{
			return boost::none;
		}

		// Check whether the support plane's sufficiently close to the portal that the origin might as well be
		// outside the shape. If so -> MISS. The distance between the two planes is n.v4 - n.v1 = n.(v4 - v1).
		double supportPlaneOffset = n.dot(v4 - v1);
		if(supportPlaneOffset < EPSILON)
		{
			return boost::none;
		}

		// Find a new portal which is closer to the surface of the shape than the old one.
		// This involves constructing a tetrahedron from the support point and the existing
		// portal, then working out which of the outer faces of the tetrahedron the origin
		// ray passes through - this outer face becomes the new portal. To do this, we look
		// at the dividing planes containing (v0,vi,v4) for i in {1,2,3} and work out which
		// segment of the tetrahedron the origin is in by testing it against them. Note that
		// the displacement value obtained by plugging the origin into dividing plane i can
		// be calculated as -v0.((vi-v0) x (v4-v0)) = -v0.vi x v4.

		double d1 = -v0.dot(v1.cross(v4));
		double d2 = -v0.dot(v2.cross(v4));
		double d3 = -v0.dot(v3.cross(v4));

		if(d1 < 0)	// the origin's behind dividing plane 1
		{
			if(d2 < 0)	// the origin's behind dividing plane 2
			{
				v1 = v4;
			}
			else		// the origin's in front of dividing plane 2
			{
				v3 = v4;
			}
		}
		else		// the origin's in front of dividing plane 1
		{
			if(d3 < 0)	// the origin's behind dividing plane 3
			{
				v2 = v4;
			}
			else		// the origin's in front of dividing plane 3
			{
				v1 = v4;
			}
		}
	}

	// If we get here, we must have run out of iterations, so just return a MISS.
	return boost::none;
}
catch(Exception&)
{
	// None of the normalizations should fail, but just in case they do, it's better to miss a collision
	// than risk crashing the entire program.
	return boost::none;
}

}
