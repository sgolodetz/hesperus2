/***
 * hesperus: OnionTree.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ONIONTREE
#define H_HESP_ONIONTREE

#include "OnionBranch.h"
#include "OnionLeaf.h"

namespace hesp {

//#################### TYPEDEFS ####################
typedef shared_ptr<class OnionTree> OnionTree_Ptr;
typedef shared_ptr<const class OnionTree> OnionTree_CPtr;

class OnionTree
{
	//#################### TYPEDEFS ####################
public:
	typedef OnionBranch Branch;
	typedef OnionLeaf Leaf;
	typedef OnionNode Node;

	//#################### PRIVATE VARIABLES ####################
private:
	std::vector<OnionNode_Ptr> m_nodes;
	std::vector<OnionLeaf*> m_leaves;
	int m_mapCount;

	//#################### CONSTRUCTORS ####################
public:
	OnionTree(const std::vector<OnionNode_Ptr>& nodes, int mapCount);

	//#################### PUBLIC METHODS ####################
public:
	const OnionLeaf *leaf(int n) const;
	static OnionTree_Ptr load_postorder_text(std::istream& is);
	int map_count() const;
	void output_postorder_text(std::ostream& os) const;
	OnionNode_Ptr root();
	OnionNode_CPtr root() const;

	//#################### PRIVATE METHODS ####################
private:
	void index_leaves();
	void index_leaves_sub(const OnionNode_Ptr& node);
};

}

#endif
