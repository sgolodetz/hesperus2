/***
 * hesperus: BSPLeaf.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_BSPLEAF
#define H_HESP_BSPLEAF

#include <vector>

#include "BSPNode.h"

namespace hesp {

class BSPLeaf : public BSPNode
{
	//#################### FRIENDS ####################
	friend class BSPTree;

	//#################### PRIVATE VARIABLES ####################
private:
	int m_leafIndex;	// leaf index of leaf (as opposed to its tree node index): only empty leaves are indexed
	bool m_isSolid;
	std::vector<int> m_polygonIndices;

	//#################### CONSTRUCTORS ####################
private:
	BSPLeaf(int index) : BSPNode(index), m_leafIndex(-1) {}

	//#################### STATIC FACTORY METHODS ####################
public:
	static BSPNode_Ptr make_empty_leaf(int index, const std::vector<int>& polygonIndices);
	static BSPNode_Ptr make_solid_leaf(int index);

	//#################### PUBLIC METHODS ####################
public:
	void add_polygon_index(int polyIndex);
	BSPBranch *as_branch();
	const BSPBranch *as_branch() const;
	BSPLeaf *as_leaf();
	const BSPLeaf *as_leaf() const;
	bool is_leaf() const;
	bool is_solid() const;
	int leaf_index() const;
	void output_postorder_text(std::ostream& os) const;
	const std::vector<int>& polygon_indices() const;

	//#################### PRIVATE METHODS ####################
private:
	void set_leaf_index(int leafIndex);
};

}

#endif
