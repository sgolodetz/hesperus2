/***
 * hesperus: OnionPortalGenerator.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "OnionPortalGenerator.h"

#include <algorithm>

namespace hesp {

/**
Clips the portal to the leaf and returns a list of portal fragments which survive the clipping process.

@param portal				The portal to clip
@param leaf					The leaf
@param relativeToPortal		The location of the subspace represented by the leaf relative to the portal (in front, behind, or straddling it)
@return						As stated
*/
std::list<OnionPortal_Ptr> OnionPortalGenerator::clip_portal_to_leaf(const OnionPortal_Ptr& portal, const OnionLeaf *leaf,
																	 PlaneClassifier relativeToPortal) const
{
	// If the leaf isn't empty in any of the maps, we can safely discard this portal.
	if(leaf->is_completely_solid()) return OnionPortalList();

	int& fromLeaf = portal->auxiliary_data().fromLeaf;
	int& toLeaf = portal->auxiliary_data().toLeaf;
	std::set<int>& fromMapIndices = portal->auxiliary_data().fromMapIndices;
	std::set<int>& toMapIndices = portal->auxiliary_data().toMapIndices;

	switch(relativeToPortal)
	{
		case CP_BACK:
		{
			fromLeaf = leaf->leaf_index();
			fromMapIndices = leaf->map_indices();
			break;
		}
		case CP_FRONT:
		{
			toLeaf = leaf->leaf_index();
			toMapIndices = leaf->map_indices();
			break;
		}
		default:	// CP_STRADDLE (note that CP_COPLANAR is not possible here)
		{
			// The portal fragment is in the middle of a leaf (this is not an error, but we do need to
			// discard the portal fragment as we'd otherwise have a portal linking a leaf to itself).
			return OnionPortalList();
		}
	}

	if(fromLeaf != -1 && toLeaf != -1)
	{
		// Both sides have been processed: we now need to decide whether or not this
		// is a valid portal fragment. To do this, we compute the set of map indices
		// shared by the leaves on either side of the portal. If it's empty (i.e. the
		// two leaves don't have a map in common), we discard the portal.

		std::set<int>& mapIndices = portal->auxiliary_data().mapIndices;
		std::set_intersection(fromMapIndices.begin(), fromMapIndices.end(),
							  toMapIndices.begin(), toMapIndices.end(),
							  std::inserter(mapIndices, mapIndices.begin()));

		if(mapIndices.empty()) return OnionPortalList();
	}

	// If we got here, either we've only processed one side of the portal (in which
	// case we want to hang on to it for now), or we've processed both sides and the
	// portal is valid. Either way, we return the portal as a singleton list.
	OnionPortalList ret;
	ret.push_back(portal);
	return ret;
}

}
