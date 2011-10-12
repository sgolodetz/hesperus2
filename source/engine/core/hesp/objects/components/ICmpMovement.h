/***
 * hesperus: ICmpMovement.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICMPMOVEMENT
#define H_HESP_ICMPMOVEMENT

#include <hesp/math/vectors/Vector3.h>
#include <hesp/objects/base/ObjectComponent.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<const class NavMesh> NavMesh_CPtr;

class ICmpMovement : public ObjectComponent
{
	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual bool attempt_navmesh_acquisition(const NavMesh_CPtr& navMesh) = 0;
	virtual int cur_nav_poly_index() const = 0;
	virtual void move(const Vector3d& dir, double speed, int milliseconds) = 0;
	virtual double run_speed() const = 0;
	virtual void set_navmesh_unacquired() = 0;
	virtual bool single_move(const Vector3d& dir, double speed, int milliseconds) = 0;
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
