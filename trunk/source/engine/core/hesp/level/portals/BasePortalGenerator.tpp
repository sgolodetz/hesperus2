/***
 * hesperus: BasePortalGenerator.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#define BPG_HEADER	template <typename PortalT, typename TreeT>
#define BPG_THIS	BasePortalGenerator<PortalT, TreeT>

#include <hesp/level/trees/TreeUtil.h>

namespace hesp {

//#################### PUBLIC METHODS ####################
/**
Generates a list of portals for a 3D world.

@param tree		The tree for the world
@return			As stated
*/
BPG_HEADER
typename BPG_THIS::PortalTList_Ptr
BPG_THIS::generate_portals(const TreeT_CPtr& tree) const
{
	PortalTList_Ptr portals(new PortalTList);
	std::list<Plane_CPtr> planes = find_unique_planes(TreeUtil::split_planes(tree));

	for(std::list<Plane_CPtr>::const_iterator it=planes.begin(), iend=planes.end(); it!=iend; ++it)
	{
		PortalT_Ptr portal = make_initial_portal(**it);
		PortalTList result = clip_portal_to_tree(portal, tree);
		portals->splice(portals->end(), result);
	}

	// Generate the opposite-facing portals.
	for(typename PortalTList::iterator it=portals->begin(), iend=portals->end(); it!=iend; ++it)
	{
		PortalT_Ptr portal = *it;

		// Construct the reverse portal.
		PortalT_Ptr reversePortal(portal->flipped_winding());
		reversePortal->auxiliary_data() = portal->auxiliary_data().flip();

		// Insert it after the existing portal in the list.
		++it;
		it = portals->insert(it, reversePortal);
	}

	return portals;
}

//#################### PRIVATE METHODS ####################
/**
Clips the portal to the subtree and returns a list of portal fragments which survive the clipping process.

@param portal				The portal to clip
@param subtreeRoot			The root of the subtree
@param relativeToPortal		The location of the subspace represented by the subtree relative to the portal (in front, behind, or straddling it)
@return						As stated
*/
BPG_HEADER
typename BPG_THIS::PortalTList
BPG_THIS::clip_portal_to_subtree(const PortalT_Ptr& portal, const NodeT_CPtr& subtreeRoot, PlaneClassifier relativeToPortal) const
{
	if(subtreeRoot->is_leaf())
	{
		return clip_portal_to_leaf(portal, subtreeRoot->as_leaf(), relativeToPortal);
	}
	else
	{
		const BranchT *branch = subtreeRoot->as_branch();
		switch(classify_polygon_against_plane(*portal, *branch->splitter()))
		{
			case CP_BACK:
			{
				return clip_portal_to_subtree(portal, branch->right(), relativeToPortal);
			}
			case CP_COPLANAR:
			{
				NodeT_CPtr fromSubtree;
				NodeT_CPtr toSubtree;
				if(branch->splitter()->normal().dot(portal->normal()) > 0)
				{
					fromSubtree = branch->right();
					toSubtree = branch->left();
				}
				else
				{
					fromSubtree = branch->left();
					toSubtree = branch->right();
				}
				PortalTList fromPortals = clip_portal_to_subtree(portal, fromSubtree, CP_BACK);
				PortalTList ret;
				for(typename PortalTList::const_iterator it=fromPortals.begin(), iend=fromPortals.end(); it!=iend; ++it)
				{
					PortalTList result = clip_portal_to_subtree(*it, toSubtree, CP_FRONT);
					ret.splice(ret.end(), result);
				}
				return ret;
			}
			case CP_FRONT:
			{
				return clip_portal_to_subtree(portal, branch->left(), relativeToPortal);
			}
			case CP_STRADDLE:
			{
				// Note: The leaf links for the two half polygons are inherited from the original polygon here.
				typedef typename PortalT::Vert PortalTVert;
				typedef typename PortalT::AuxData PortalTAuxData;
				SplitResults<PortalTVert,PortalTAuxData> sr = split_polygon(*portal, *branch->splitter());

				PortalTList frontResult = clip_portal_to_subtree(sr.front, branch->left(), relativeToPortal);
				PortalTList backResult = clip_portal_to_subtree(sr.back, branch->right(), relativeToPortal);

				PortalTList ret;
				ret.splice(ret.end(), frontResult);
				ret.splice(ret.end(), backResult);
				return ret;
			}
		}
	}

	// The code will never actually get here, because the switch above is exhaustive,
	// but the compiler still warns us because it can't tell that.
	throw Exception("This should never happen");
}

/**
Clips the portal to the tree and returns a list of portal fragments which survive the clipping process.

@param portal	The portal to clip
@param tree		The tree to which to clip it
@return			The list of portal fragments which survived the clipping process
*/
BPG_HEADER
typename BPG_THIS::PortalTList
BPG_THIS::clip_portal_to_tree(const PortalT_Ptr& portal, const TreeT_CPtr& tree) const
{
	return clip_portal_to_subtree(portal, tree->root());
}

/**
Makes an initial portal on a given plane. This portal should be large enough to
span the entire level space.

@param plane	The plane on which to build the initial portal
@return			As stated
*/
BPG_HEADER
typename BPG_THIS::PortalT_Ptr
BPG_THIS::make_initial_portal(const Plane& plane) const
{
	return make_universe_polygon<typename PortalT::AuxData>(plane);
}

}

#undef BPG_THIS
#undef BPG_HEADER
