/***
 * hesperus: OnionBranch.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ONIONBRANCH
#define H_HESP_ONIONBRANCH

#include "OnionNode.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<const class Plane> Plane_CPtr;

class OnionBranch : public OnionNode
{
	//#################### PRIVATE VARIABLES ####################
private:
	OnionNode_Ptr m_left;
	OnionNode_Ptr m_right;
	Plane_CPtr m_splitter;

	//#################### CONSTRUCTORS ####################
public:
	OnionBranch(int index, const Plane_CPtr& splitter, const OnionNode_Ptr& left, const OnionNode_Ptr& right);

	//#################### PUBLIC METHODS ####################
public:
	OnionBranch *as_branch();
	const OnionBranch *as_branch() const;
	OnionLeaf *as_leaf();
	const OnionLeaf *as_leaf() const;
	bool is_leaf() const;
	const OnionNode_Ptr& left();
	OnionNode_CPtr left() const;
	void output_postorder_text(std::ostream& os) const;
	const OnionNode_Ptr& right();
	OnionNode_CPtr right() const;
	Plane_CPtr splitter() const;
};

}

#endif
