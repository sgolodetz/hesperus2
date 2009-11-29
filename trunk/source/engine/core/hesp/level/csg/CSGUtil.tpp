/***
 * hesperus: CSGUtil.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#define CSGUtil_HEADER	template <typename Vert, typename AuxData>
#define CSGUtil_THIS	CSGUtil<Vert,AuxData>

#include <hesp/level/trees/BSPBranch.h>

namespace hesp {

//#################### PUBLIC METHODS ####################
/**
Clips a list of polygons to a tree.

@param polys			The polygons
@param tree				The tree
@param coplanarFlag		Whether same-facing coplanar polygons should be passed down the front side of the tree
@return					A list of polygon fragments that survived the clipping process
*/
CSGUtil_HEADER
typename CSGUtil_THIS::PolyList
CSGUtil_THIS::clip_polygons_to_tree(const PolyList& polys, const BSPTree_CPtr& tree, bool coplanarFlag)
{
	PolyList ret;
	for(typename PolyList::const_iterator it=polys.begin(), iend=polys.end(); it!=iend; ++it)
	{
		std::pair<PolyList,bool> result = clip_polygon_to_subtree(*it, tree->root(), coplanarFlag);
		ret.splice(ret.end(), result.first);
	}
	return ret;
}

/**
Unions the polygons in a set of convex brushes to produce a valid set of polygon geometry

@param brushes	The brushes
@return			The valid polygon geometry
*/
CSGUtil_HEADER
typename CSGUtil_THIS::PolyList_Ptr
CSGUtil_THIS::union_all(const PolyBrushVector& brushes)
{
	PolyList_Ptr ret(new PolyList);

	// Build a tree for each brush.
	int brushCount = static_cast<int>(brushes.size());
	std::vector<BSPTree_Ptr> trees(brushCount);
	for(int i=0; i<brushCount; ++i)
	{
		trees[i] = build_tree(*brushes[i]);
	}

	// Determine which brushes can interact with each other.
	const double TOLERANCE = 1.0;
	std::vector<std::vector<unsigned char> > brushesInteract(brushCount);
	for(int i=0; i<brushCount; ++i)
	{
		brushesInteract[i].reserve(brushCount);
		for(int j=0; j<brushCount; ++j)
		{
			if(j == i) brushesInteract[i].push_back(false);
			else brushesInteract[i].push_back(AABB3d::within_range(brushes[i]->bounds(), brushes[j]->bounds(), TOLERANCE));
		}
	}

	// Clip each polygon to the tree of each brush within range of its own brush.
	for(int i=0; i<brushCount; ++i)
	{
		const PolyVector& faces = brushes[i]->faces();
		int faceCount = static_cast<int>(faces.size());
		for(int j=0; j<faceCount; ++j)
		{
			PolyList fragments;
			fragments.push_back(faces[j]);
			for(int k=0; k<brushCount; ++k)
			{
				if(brushesInteract[i][k])
				{
					fragments = clip_polygons_to_tree(fragments, trees[k], i < k);
				}
			}
			ret->splice(ret->end(), fragments);
		}
	}

	return ret;
}

//#################### PRIVATE METHODS ####################
/**
Builds a BSP tree for the specified brush. Since the brush is
a convex polyhedron, the tree will be right-linear and can be
constructed relatively easily.

@param brush	The brush
@return			A right-linear tree for it
*/
CSGUtil_HEADER
BSPTree_Ptr CSGUtil_THIS::build_tree(const PolyBrush& brush)
{
	const PolyVector& faces = brush.faces();
	int faceCount = static_cast<int>(faces.size());

	int nodeCount = 2*faceCount+1;
	std::vector<BSPNode_Ptr> nodes(nodeCount);

	for(int i=0; i<faceCount; ++i) nodes[i] = BSPLeaf::make_empty_leaf(i, std::vector<int>());
	nodes[faceCount] = BSPLeaf::make_solid_leaf(faceCount);
	for(int i=faceCount+1; i<nodeCount; ++i)
	{
		Plane_Ptr splitter(new Plane(make_plane(*faces[nodeCount-i-1])));
		nodes[i].reset(new BSPBranch(i, splitter, nodes[nodeCount-i-1], nodes[i-1]));
	}

	return BSPTree_Ptr(new BSPTree(nodes));
}

/**
Clips a polygon to a subtree.

@param poly				The input polygon
@param node				The root node of the subtree
@param coplanarFlag		Whether same-facing coplanar polygons should be passed down the front side of the tree
@return					A pair, the first component of which is a list of polygon fragments that survived the
						clipping process, and the second component of which is a bool indicating whether the
						whole input polygon survived the clip
*/
CSGUtil_HEADER
std::pair<typename CSGUtil_THIS::PolyList, bool>
CSGUtil_THIS::clip_polygon_to_subtree(const Poly_Ptr& poly, const BSPNode_CPtr& node, bool coplanarFlag)
{
	if(node->is_leaf())
	{
		const BSPLeaf *leaf = node->as_leaf();

		PolyList ret;
		if(leaf->is_solid())
		{
			// The polygon ended up in a solid leaf, so discard it.
			return std::make_pair(ret, false);
		}
		else
		{
			// The polygon ended up in an empty leaf, so keep it.
			ret.push_back(poly);
			return std::make_pair(ret, true);
		}
	}
	else
	{
		const BSPBranch *branch = node->as_branch();
		switch(classify_polygon_against_plane(*poly, *branch->splitter()))
		{
			case CP_BACK:
			{
				return clip_polygon_to_subtree(poly, branch->right(), coplanarFlag);
			}
			case CP_COPLANAR:
			{
				bool sameFacing = poly->normal().dot(branch->splitter()->normal()) > 0;
				if(sameFacing && coplanarFlag) return clip_polygon_to_subtree(poly, branch->left(), coplanarFlag);
				else return clip_polygon_to_subtree(poly, branch->right(), coplanarFlag);
			}
			case CP_FRONT:
			{
				return clip_polygon_to_subtree(poly, branch->left(), coplanarFlag);
			}
			default:	// CP_STRADDLE
			{
				SplitResults<Vert,AuxData> sr = split_polygon(*poly, *branch->splitter());
				std::pair<PolyList,bool> frontResult = clip_polygon_to_subtree(sr.front, branch->left(), coplanarFlag);
				std::pair<PolyList,bool> backResult = clip_polygon_to_subtree(sr.back, branch->right(), coplanarFlag);

				if(frontResult.second && backResult.second)
				{
					// Both halves of the fragment survived, so keep the original fragment.
					PolyList ret;
					ret.push_back(poly);
					return std::make_pair(ret, true);
				}
				else
				{
					PolyList ret;
					ret.splice(ret.end(), frontResult.first);
					ret.splice(ret.end(), backResult.first);
					return std::make_pair(ret, false);
				}
			}
		}
	}
}

}

#undef CSGUtil_THIS
#undef CSGUtil_HEADER
