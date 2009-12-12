/***
 * hesperus: ICmpMovement.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICMPMOVEMENT
#define H_HESP_ICMPMOVEMENT

#include <hesp/objects/base/ObjectComponent.h>
#include <hesp/util/PolygonTypes.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<const class NavManager> NavManager_CPtr;
typedef shared_ptr<const class NavMesh> NavMesh_CPtr;
typedef shared_ptr<const class OnionTree> OnionTree_CPtr;

class ICmpMovement : public ObjectComponent
{
	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual bool attempt_navmesh_acquisition(const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_CPtr& tree, const NavMesh_CPtr& navMesh) = 0;
	virtual int cur_nav_poly_index() const = 0;
	virtual void move(const Vector3d& dir, double speed, int milliseconds, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_CPtr& tree, const NavManager_CPtr& navManager) = 0;
	virtual double run_speed() const = 0;
	virtual void set_navmesh_unacquired() = 0;
	virtual bool single_move(const Vector3d& dir, double speed, int milliseconds, const OnionTree_CPtr& tree) = 0;
	virtual bool traversing_link() const = 0;
	virtual double walk_speed() const = 0;

	//#################### PUBLIC METHODS ####################
public:
	std::string group_type() const			{ return "Movement"; }
	static std::string static_group_type()	{ return "Movement"; }

	std::string own_type() const			{ return "Movement"; }
	static std::string static_own_type()	{ return "Movement"; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ICmpMovement> ICmpMovement_Ptr;
typedef shared_ptr<const ICmpMovement> ICmpMovement_CPtr;

}

#endif
