/***
 * hesperus: ICmpBasicModelRender.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ICMPBASICMODELRENDER
#define H_HESP_ICMPBASICMODELRENDER

#include "ICmpModelRender.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class BoneHierarchy> BoneHierarchy_Ptr;
typedef shared_ptr<const class RBTMatrix> RBTMatrix_CPtr;

class ICmpBasicModelRender : public virtual ICmpModelRender
{
	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void render_child() const = 0;
	virtual void update_child_animation(int milliseconds, const BoneHierarchy_Ptr& parent, const std::string& parentBoneName, const RBTMatrix_CPtr& parentMatrix, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_CPtr& tree, const NavManager_CPtr& navManager) = 0;

	//#################### PUBLIC METHODS ####################
public:
	std::string own_type() const			{ return "BasicModelRender"; }
	static std::string static_own_type()	{ return "BasicModelRender"; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ICmpBasicModelRender> ICmpBasicModelRender_Ptr;
typedef shared_ptr<const ICmpBasicModelRender> ICmpBasicModelRender_CPtr;

}

#endif
