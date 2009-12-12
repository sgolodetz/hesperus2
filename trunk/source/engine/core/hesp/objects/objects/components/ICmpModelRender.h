/***
 * hesperus: ICmpModelRender.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICMPMODELRENDER
#define H_HESP_ICMPMODELRENDER

#include <hesp/util/PolygonTypes.h>
#include "ICmpRender.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<const class NavManager> NavManager_CPtr;
typedef shared_ptr<const class OnionTree> OnionTree_CPtr;

class ICmpModelRender : public ICmpRender
{
	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void render_first_person() const = 0;
	virtual void set_highlights(bool enabled) = 0;
	virtual void update_animation(int milliseconds, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_CPtr& tree, const NavManager_CPtr& navManager) = 0;

	//#################### PUBLIC METHODS ####################
public:
	std::string own_type() const			{ return "ModelRender"; }
	static std::string static_own_type()	{ return "ModelRender"; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ICmpModelRender> ICmpModelRender_Ptr;
typedef shared_ptr<const ICmpModelRender> ICmpModelRender_CPtr;

}

#endif
