/***
 * hesperus: CmpMovement.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpMovement.h"

#include <hesp/bounds/BoundsManager.h>
#include <hesp/database/Database.h>
#include <hesp/math/geom/GeomUtil.h>
#include <hesp/nav/NavDataset.h>
#include <hesp/nav/NavLink.h>
#include <hesp/nav/NavManager.h>
#include <hesp/nav/NavMesh.h>
#include <hesp/nav/NavMeshUtil.h>
#include <hesp/nav/NavPolygon.h>
#include <hesp/trees/OnionUtil.h>
#include "ICmpSimulation.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpMovement::CmpMovement()
:	m_curNavPolyIndex(-1)
{}

//#################### STATIC FACTORY METHODS ####################
IObjectComponent_Ptr CmpMovement::load(const Properties&)
{
	return IObjectComponent_Ptr(new CmpMovement);
}

//#################### PUBLIC METHODS ####################
bool CmpMovement::attempt_navmesh_acquisition(const NavMesh_CPtr& navMesh)
{
	const Database& db = *m_objectManager->database();
	shared_ptr<const std::vector<CollisionPolygon_Ptr> > polygons = db.get("db://OnionPolygons", polygons);
	OnionTree_CPtr tree = db.get("db://OnionTree", tree);

	ICmpPosition_CPtr cmpPosition = m_objectManager->get_component(m_objectID, cmpPosition);	assert(cmpPosition != NULL);
	const Vector3d& position = cmpPosition->position();

	// Try and find a nav polygon, starting from the last known one.
	m_curNavPolyIndex = NavMeshUtil::find_nav_polygon(position, m_curNavPolyIndex, *polygons, tree, navMesh);

	return m_curNavPolyIndex != -1;
}

void CmpMovement::check_dependencies() const
{
	check_dependency<ICmpSimulation>();
}

int CmpMovement::cur_nav_poly_index() const
{
	return m_curNavPolyIndex;
}

void CmpMovement::move(const Vector3d& dir, double speed, int milliseconds)
{
	NavManager_CPtr navManager = m_objectManager->database()->get("db://NavManager", navManager);

	ICmpSimulation_Ptr cmpSimulation = m_objectManager->get_component(m_objectID, cmpSimulation);	assert(cmpSimulation != NULL);

	Move move;
	move.dir = dir;
	move.mapIndex = m_objectManager->bounds_manager()->lookup_bounds_index(cmpSimulation->bounds_group(), cmpSimulation->posture());
	move.timeRemaining = milliseconds / 1000.0;

	NavMesh_CPtr navMesh = navManager->dataset(move.mapIndex)->nav_mesh();

	double oldTimeRemaining;
	do
	{
		oldTimeRemaining = move.timeRemaining;

		if(m_curTraversal) do_traverse_move(move, speed /* FIXME: Select the appropriate speed here */, navMesh);
		if(move.timeRemaining == 0) break;

		if(attempt_navmesh_acquisition(navMesh)) do_navmesh_move(move, speed, navMesh);
		else do_direct_move(move, speed);
	} while(move.timeRemaining > 0 && oldTimeRemaining - move.timeRemaining > 0.0001);
}

double CmpMovement::run_speed() const
{
	// FIXME: This should be loaded in.
	return 10.0;	// in units/s
}

Properties CmpMovement::save() const
{
	return Properties();
}

/**
@return	true, if a collision occurred, or false otherwise
*/
bool CmpMovement::single_move(const Vector3d& dir, double speed, int milliseconds)
{
	// FIXME: The bool return here is unintuitive and should be replaced with something more sensible.

	ICmpSimulation_Ptr cmpSimulation = m_objectManager->get_component(m_objectID, cmpSimulation);	assert(cmpSimulation != NULL);

	// Check to make sure we're not currently traversing a link: don't let the object be moved if we are.
	if(m_curTraversal) return true;

	Move move;
	move.dir = dir;
	move.mapIndex = m_objectManager->bounds_manager()->lookup_bounds_index(cmpSimulation->bounds_group(), cmpSimulation->posture());
	move.timeRemaining = milliseconds / 1000.0;

	return do_direct_move(move, speed);
}

void CmpMovement::set_navmesh_unacquired()
{
	m_curNavPolyIndex = -1;
}

bool CmpMovement::traversing_link() const
{
	return m_curTraversal != NULL;
}

double CmpMovement::walk_speed() const
{
	// FIXME: This should be loaded in.
	return 5.0;		// in units/s
}

//#################### PRIVATE METHODS ####################
/**
@return	true, if a collision occurred, or false otherwise
*/
bool CmpMovement::do_direct_move(Move& move, double speed)
{
	bool collisionOccurred = false;

	OnionTree_CPtr tree = m_objectManager->database()->get("db://OnionTree", tree);

	ICmpSimulation_Ptr cmpSimulation = m_objectManager->get_component(m_objectID, cmpSimulation);	assert(cmpSimulation != NULL);

	Vector3d source = cmpSimulation->position();
	Vector3d dest = source + move.dir * speed * move.timeRemaining;

	// Check the ray against the tree.
	OnionUtil::Transition transition = OnionUtil::find_first_transition(move.mapIndex, source, dest, tree);
	switch(transition.classifier)
	{
		case OnionUtil::RAY_EMPTY:
		{
			// It's perfectly fine to let the object move along this ray, as it doesn't intersect a wall.
			break;
		}
		case OnionUtil::RAY_SOLID:
		{
			// We were on a wall (i.e. coplanar to it) prior to the move - prevent any further moves into the wall,
			// and update the move direction to allow sliding along the wall instead.
			update_move_direction_for_sliding(move);
			move.timeRemaining -= 0.001;	// make this cost 1ms of time (otherwise the calling function will think we got stuck)
			return true;
		}
		case OnionUtil::RAY_TRANSITION_ES:
		{
			// Stop the object going into a wall.
			dest = *transition.location;
			collisionOccurred = true;

			// Record the transition plane.
			update_recent_planes(*transition.plane);

			// Update the move direction to allow sliding.
			update_move_direction_for_sliding(move);

			break;
		}
		case OnionUtil::RAY_TRANSITION_SE:
		{
			// This should never happen (since objects can't move into walls), but better let the object back into
			// the world if it does happen.
			break;
		}
	}

	cmpSimulation->set_position(dest);

	// Update the time remaining.
	double moveLength = source.distance(dest);
	double timeTaken = moveLength / speed;
	move.timeRemaining -= timeTaken;

	return collisionOccurred;
}

void CmpMovement::do_navmesh_move(Move& move, double speed, const NavMesh_CPtr& navMesh)
{
	shared_ptr<const std::vector<CollisionPolygon_Ptr> > polygons = m_objectManager->database()->get("db://OnionPolygons", polygons);

	ICmpPosition_Ptr cmpPosition = m_objectManager->get_component(m_objectID, cmpPosition);		assert(cmpPosition != NULL);

	// Step 1:		Project the movement vector onto the plane of the current nav polygon.

	const NavPolygon& navPoly = *navMesh->polygons()[m_curNavPolyIndex];
	int curColPolyIndex = navPoly.collision_poly_index();
	const CollisionPolygon& curPoly = *(*polygons)[curColPolyIndex];
	Plane plane = make_plane(curPoly);
	move.dir = project_vector_onto_plane(move.dir, plane);
	if(move.dir.length_squared() > SMALL_EPSILON*SMALL_EPSILON) move.dir.normalize();

	// Step 2:		Check whether the new movement vector goes through the influence zone of any of the out navlinks.

	Vector3d source = cmpPosition->position();
	Vector3d dest = source + move.dir * speed * move.timeRemaining;

	boost::optional<Vector3d> hit;
	int hitNavlink = -1;
	const std::vector<int>& links = navPoly.out_links();
	for(std::vector<int>::const_iterator it=links.begin(), iend=links.end(); it!=iend; ++it)
	{
		const NavLink_Ptr& link = navMesh->links()[*it];
		hit = link->hit_test(source, dest);

		if(hit)
		{
			hitNavlink = *it;
#if 0
			std::cout << "Hit navlink at " << *hit << ": ";
			link->output(std::cout);
			std::cout << '\n';
#endif
			break;
		}
	}

	// Step 3.a:	If the new movement vector doesn't hit a navlink, check whether the other end of the movement vector is within the current polygon.
	//
	//				-	If yes, move there, set the time remaining to zero and return.
	//
	//				-	If no, do a direct move in the original direction, since we are either leaving the navmesh or hitting a wall.

	if(hitNavlink == -1)
	{
		if(point_in_polygon(dest, curPoly))
		{
			cmpPosition->set_position(dest);
			move.timeRemaining = 0;
		}
		else
		{
			do_direct_move(move, speed);
		}

		return;
	}

	// Step 3.b:	If the new movement vector hits a navlink, move the point at which it first enters the influence zone,
	//				and reduce the time remaining appropriately. Then, initiate the link traversal.

	// Move the object to the link entrance point.
	cmpPosition->set_position(*hit);

	// Update the time remaining.
	double moveLength = source.distance(*hit);
	double timeTaken = moveLength / speed;
	move.timeRemaining -= timeTaken;

	// Initiate the link traversal.
	m_curTraversal.reset(new Traversal(hitNavlink, *hit, 0));
}

void CmpMovement::do_traverse_move(Move& move, double speed, const NavMesh_CPtr& navMesh)
{
	shared_ptr<const std::vector<CollisionPolygon_Ptr> > polygons = m_objectManager->database()->get("db://OnionPolygons", polygons);

	ICmpPosition_Ptr cmpPosition = m_objectManager->get_component(m_objectID, cmpPosition);		assert(cmpPosition != NULL);

	Traversal_CPtr traversal = m_curTraversal;
	if(!traversal) return;

	NavLink_Ptr link = navMesh->links()[traversal->linkIndex];
	double remaining = 1 - traversal->t;													// % of link remaining
	double remainingTraversalTime = remaining * link->traversal_time(speed);				// time to traverse remainder
	double availableTraversalTime = std::min(remainingTraversalTime, move.timeRemaining);	// time to spend traversing

	if(availableTraversalTime >= remainingTraversalTime)
	{
		// Finish traversing the link:

		// Update the current nav polygon and clear the current traversal.
		m_curNavPolyIndex = link->dest_poly();
		m_curTraversal.reset();

		// Move to an exit point on the link.
		Vector3d dest = link->traverse(traversal->source, 1);
		cmpPosition->set_position(dest);
		move.timeRemaining -= remainingTraversalTime;
		
#if 0
		int colPolyIndex = navMesh->polygons()[link->dest_poly()]->collision_poly_index();
		std::cout << "Linked to polygon (" << colPolyIndex << ',' << link->dest_poly() << ')' << std::endl;
#endif

		// Move the object very slightly away from the navlink exit: this is a hack to prevent link loops.
		int destColPolyIndex = navMesh->polygons()[link->dest_poly()]->collision_poly_index();
		const CollisionPolygon& destPoly = *(*polygons)[destColPolyIndex];
		Plane destPlane = make_plane(destPoly);
		Vector3d destDir = project_vector_onto_plane(move.dir, destPlane);
		dest += destDir * 0.001;
		if(point_in_polygon(dest, destPoly)) cmpPosition->set_position(dest);
	}
	else
	{
		// Work out how much further we've progressed and update the traversal field accordingly.
		double deltaT = (availableTraversalTime / remainingTraversalTime) * remaining;
		Traversal_CPtr newTraversal(new Traversal(traversal->linkIndex, traversal->source, traversal->t + deltaT));
		m_curTraversal = newTraversal;

		// Move further along the link.
		Vector3d dest = link->traverse(newTraversal->source, newTraversal->t);
		cmpPosition->set_position(dest);
		move.timeRemaining = 0;
	}
}

void CmpMovement::update_move_direction_for_sliding(Move& move)
{
	// Update the move direction to be along the wall (to allow sliding). To do this, we remove the
	// component of the movement which is normal to the wall. To find the wall we're on, we look at
	// all the 'recent' planes we've touched and choose one which we're trying to walk behind.

	ICmpSimulation_CPtr cmpSimulation = m_objectManager->get_component(m_objectID, cmpSimulation);	assert(cmpSimulation != NULL);
	
	const Vector3d& source = cmpSimulation->position();
	Vector3d dummyDest = source + move.dir;
	for(std::list<Plane>::const_iterator it=m_recentPlanes.begin(), iend=m_recentPlanes.end(); it!=iend; ++it)
	{
		if(classify_point_against_plane(dummyDest, *it) == CP_BACK)
		{
			move.dir = project_vector_onto_plane(move.dir, *it);
			if(move.dir.length() > SMALL_EPSILON) move.dir.normalize();
			else move.timeRemaining = 0;

			break;
		}
	}
}

void CmpMovement::update_recent_planes(const Plane& plane)
{
	ICmpSimulation_CPtr cmpSimulation = m_objectManager->get_component(m_objectID, cmpSimulation);	assert(cmpSimulation != NULL);

	// Remove any recent planes which the object's no longer on.
	const Vector3d& position = cmpSimulation->position();
	for(std::list<Plane>::iterator it=m_recentPlanes.begin(), iend=m_recentPlanes.end(); it!=iend;)
	{
		if(classify_point_against_plane(position, *it) == CP_COPLANAR) ++it;
		else it = m_recentPlanes.erase(it);
	}

	// Add the latest plane.
	m_recentPlanes.push_front(plane);
}

}
