/***
 * hesperus: CmpBipedAnimChooser.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMPBIPEDANIMCHOOSER
#define H_HESP_CMPBIPEDANIMCHOOSER

#include "ICmpBipedAnimChooser.h"
#include "ICmpHealth.h"

namespace hesp {

class CmpBipedAnimChooser : public ICmpBipedAnimChooser
{
	//#################### ENUMERATIONS ####################
private:
	enum MovementType
	{
		UNKNOWN,
		AIR,
		IDLE,
		RUN,
		WALK,
	};

	//#################### PRIVATE VARIABLES ####################
private:
	bool m_runFlag;
	bool m_walkFlag;

	//#################### CONSTRUCTORS ####################
public:
	CmpBipedAnimChooser();

	//#################### STATIC FACTORY METHODS ####################
public:
	static IObjectComponent_Ptr load(const Properties&);

	//#################### PUBLIC METHODS ####################
public:
	void check_dependencies() const;
	std::string choose_animation(const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_CPtr& tree, const NavManager_CPtr& navManager);
	Properties save() const;
	void set_run_flag();
	void set_walk_flag();

	//#################### PRIVATE METHODS ####################
private:
	std::string determine_anim_extension() const;
	bool determine_crouching() const;
	ICmpHealth::HealthStatus determine_health_status() const;
	MovementType determine_movement_type(const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_CPtr& tree, const NavManager_CPtr& navManager);
	void reset_flags();
};

}

#endif
