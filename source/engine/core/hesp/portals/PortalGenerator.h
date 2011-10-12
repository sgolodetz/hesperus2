/***
 * hesperus: PortalGenerator.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_PORTALGENERATOR
#define H_HESP_PORTALGENERATOR

#include <hesp/trees/BSPTree.h>
#include "BasePortalGenerator.h"
#include "Portal.h"

namespace hesp {

class PortalGenerator : public BasePortalGenerator<Portal,BSPTree>
{
	//#################### TYPEDEFS ####################
private:
	typedef std::list<Portal_Ptr> PortalList;
	typedef shared_ptr<PortalList> PortalList_Ptr;

	//#################### PRIVATE METHODS ####################
private:
	PortalList clip_portal_to_leaf(const Portal_Ptr& portal, const BSPLeaf *leaf, PlaneClassifier relativeToPortal) const;
};

}

#endif
