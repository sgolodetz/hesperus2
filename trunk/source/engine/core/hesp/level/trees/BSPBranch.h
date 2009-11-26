/***
 * hesperus: BSPBranch.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_BSPBRANCH
#define H_HESP_BSPBRANCH

#include "BSPNode.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<const class Plane> Plane_CPtr;

class BSPBranch : public BSPNode
{
	//#################### PRIVATE VARIABLES ####################
private:
	BSPNode_Ptr m_left;
	BSPNode_Ptr m_right;
	Plane_CPtr m_splitter;

	//#################### CONSTRUCTORS ####################
public:
	BSPBranch(int index, const Plane_CPtr& splitter, const BSPNode_Ptr& left, const BSPNode_Ptr& right);

	//#################### PUBLIC METHODS ####################
public:
	BSPBranch *as_branch();
	const BSPBranch *as_branch() const;
	BSPLeaf *as_leaf();
	const BSPLeaf *as_leaf() const;
	bool is_leaf() const;
	const BSPNode_Ptr& left();
	BSPNode_CPtr left() const;
	void output_postorder_text(std::ostream& os) const;
	const BSPNode_Ptr& right();
	BSPNode_CPtr right() const;
	Plane_CPtr splitter() const;
};

}

#endif
