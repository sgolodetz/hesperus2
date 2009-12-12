/***
 * hesperus: ICmpYoke.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICMPYOKE
#define H_HESP_ICMPYOKE

#include <hesp/objects/base/ObjectComponent.h>
#include <hesp/util/PolygonTypes.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
class InputState;
typedef shared_ptr<const class NavManager> NavManager_CPtr;
typedef shared_ptr<class ObjectCommand> ObjectCommand_Ptr;
typedef shared_ptr<const class OnionTree> OnionTree_CPtr;

class ICmpYoke : public ObjectComponent
{
	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual std::vector<ObjectCommand_Ptr> generate_commands(InputState& input, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_CPtr& tree, const NavManager_CPtr& navManager) = 0;

	//#################### PUBLIC METHODS ####################
public:
	std::string group_type() const			{ return "Yoke"; }
	static std::string static_group_type()	{ return "Yoke"; }

	std::string own_type() const			{ return "Yoke"; }
	static std::string static_own_type()	{ return "Yoke"; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ICmpYoke> ICmpYoke_Ptr;
typedef shared_ptr<const ICmpYoke> ICmpYoke_CPtr;

}

#endif
