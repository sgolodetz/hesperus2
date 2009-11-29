/***
 * hesperus: PortalGenerator.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "PortalGenerator.h"

namespace hesp {

//#################### PRIVATE METHODS ####################
/**
Clips the portal to the leaf and returns a list of portal fragments which survive the clipping process.

@param portal				The portal to clip
@param leaf					The leaf
@param relativeToPortal		The location of the subspace represented by the leaf relative to the portal (in front, behind, or straddling it)
@return						As stated
*/
std::list<Portal_Ptr> PortalGenerator::clip_portal_to_leaf(const Portal_Ptr& portal, const BSPLeaf *leaf, PlaneClassifier relativeToPortal) const
{
	if(leaf->is_solid()) return PortalList();

	switch(relativeToPortal)
	{
		case CP_BACK:
		{
			portal->auxiliary_data().fromLeaf = leaf->leaf_index();
			break;
		}
		case CP_FRONT:
		{
			portal->auxiliary_data().toLeaf = leaf->leaf_index();
			break;
		}
		default:	// CP_STRADDLE (note that CP_COPLANAR is not possible here)
		{
			// The portal fragment is in the middle of a leaf (this is not an error, but we do need to
			// discard the portal fragment as we'd otherwise have a portal linking a leaf to itself).
			return PortalList();
		}
	}
	PortalList ret;
	ret.push_back(portal);
	return ret;
}

}
