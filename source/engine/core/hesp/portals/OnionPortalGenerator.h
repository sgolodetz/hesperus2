/***
 * hesperus: OnionPortalGenerator.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ONIONPORTALGENERATOR
#define H_HESP_ONIONPORTALGENERATOR

#include <hesp/trees/OnionTree.h>
#include "BasePortalGenerator.h"
#include "OnionPortal.h"

namespace hesp {

class OnionPortalGenerator : public BasePortalGenerator<OnionPortal,OnionTree>
{
	//#################### TYPEDEFS ####################
private:
	typedef std::list<OnionPortal_Ptr> OnionPortalList;
	typedef shared_ptr<OnionPortalList> OnionPortalList_Ptr;

	//#################### PRIVATE METHODS ####################
private:
	OnionPortalList clip_portal_to_leaf(const OnionPortal_Ptr& portal, const OnionLeaf *leaf, PlaneClassifier relativeToPortal) const;
};

}

#endif
