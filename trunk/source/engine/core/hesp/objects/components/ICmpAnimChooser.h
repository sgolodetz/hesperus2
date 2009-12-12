/***
 * hesperus: ICmpAnimChooser.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICMPANIMCHOOSER
#define H_HESP_ICMPANIMCHOOSER

#include <hesp/objects/base/ObjectComponent.h>
#include <hesp/util/PolygonTypes.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<const class NavManager> NavManager_CPtr;
typedef shared_ptr<const class OnionTree> OnionTree_CPtr;

class ICmpAnimChooser : public ObjectComponent
{
	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual std::string choose_animation(const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_CPtr& tree, const NavManager_CPtr& navManager) = 0;

	//#################### PUBLIC METHODS ####################
public:
	std::string group_type() const			{ return "AnimChooser"; }
	static std::string static_group_type()	{ return "AnimChooser"; }

	std::string own_type() const			{ return "AnimChooser"; }
	static std::string static_own_type()	{ return "AnimChooser"; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ICmpAnimChooser> ICmpAnimChooser_Ptr;
typedef shared_ptr<const ICmpAnimChooser> ICmpAnimChooser_CPtr;

}

#endif
