/***
 * hesperus: CmpUserBipedYoke.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMPUSERBIPEDYOKE
#define H_HESP_CMPUSERBIPEDYOKE

#include "ICmpYoke.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class IYoke> IYoke_Ptr;

class CmpUserBipedYoke : public ICmpYoke
{
	//#################### PRIVATE VARIABLES ####################
private:
	IYoke_Ptr m_yoke;

	//#################### STATIC FACTORY METHODS ####################
public:
	static IObjectComponent_Ptr load(const Properties&);

	//#################### PUBLIC METHODS ####################
public:
	void check_dependencies() const;
	std::vector<ObjectCommand_Ptr> generate_commands(InputState& input, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_CPtr& tree, const NavManager_CPtr& navManager);
	Properties save() const;

	std::string own_type() const			{ return "UserBipedYoke"; }
	static std::string static_own_type()	{ return "UserBipedYoke"; }
};

}

#endif
