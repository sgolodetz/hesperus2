/***
 * hesperus: CmpMovement.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMPMOVEMENT
#define H_HESP_CMPMOVEMENT

#include <list>

#include <hesp/math/geom/Plane.h>
#include "ICmpMovement.h"

namespace hesp {

class CmpMovement : public ICmpMovement
{
	//#################### NESTED CLASSES ####################
private:
	struct Move
	{
		double timeRemaining;	// the time remaining for moving (in seconds)
		Vector3d dir;			// the current desired movement direction
		int mapIndex;			// which navigation map the object's on
	};

	struct Traversal
	{
		const int linkIndex;
		const Vector3d source;
		const double t;

		Traversal(int linkIndex_, const Vector3d& source_, double t_)
		:	linkIndex(linkIndex_), source(source_), t(t_)
		{}
	};

	//#################### TYPEDEFS ####################
private:
	typedef shared_ptr<const Traversal> Traversal_CPtr;

	//#################### PRIVATE VARIABLES ####################
private:
	// The index of the nav polygon in which the centre of the object's bounds currently resides (-1 if unknown)
	int m_curNavPolyIndex;

	// The details of the current nav link traversal (NULL if not currently traversing a link)
	Traversal_CPtr m_curTraversal;

	// Planes with which the object has recently been in contact
	std::list<Plane> m_recentPlanes;

	//#################### CONSTRUCTORS ####################
public:
	CmpMovement();

	//#################### STATIC FACTORY METHODS ####################
public:
	static IObjectComponent_Ptr load(const Properties&);

	//#################### PUBLIC METHODS ####################
public:
	bool attempt_navmesh_acquisition(const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_CPtr& tree, const NavMesh_CPtr& navMesh);
	void check_dependencies() const;
	int cur_nav_poly_index() const;
	void move(const Vector3d& dir, double speed, int milliseconds, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_CPtr& tree, const NavManager_CPtr& navManager);
	double run_speed() const;
	Properties save() const;
	void set_navmesh_unacquired();
	bool single_move(const Vector3d& dir, double speed, int milliseconds, const OnionTree_CPtr& tree);
	bool traversing_link() const;
	double walk_speed() const;

	//#################### PRIVATE METHODS ####################
private:
	bool do_direct_move(Move& move, double speed, const OnionTree_CPtr& tree);
	void do_navmesh_move(Move& move, double speed, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_CPtr& tree, const NavMesh_CPtr& navMesh);
	void do_traverse_move(Move& move, double speed, const std::vector<CollisionPolygon_Ptr>& polygons, const NavMesh_CPtr& navMesh);
	void update_move_direction_for_sliding(Move& move);
	void update_recent_planes(const Plane& plane);
};

}

#endif
