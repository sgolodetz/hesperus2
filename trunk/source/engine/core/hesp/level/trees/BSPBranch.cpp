/***
 * hesperus: BSPBranch.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "BSPBranch.h"

#include <hesp/math/geom/Plane.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
BSPBranch::BSPBranch(int index, const Plane_CPtr& splitter, const BSPNode_Ptr& left, const BSPNode_Ptr& right)
:	BSPNode(index), m_splitter(splitter), m_left(left), m_right(right)
{
	left->set_parent(this);
	right->set_parent(this);
}

//#################### PUBLIC METHODS ####################
BSPBranch *BSPBranch::as_branch()				{ return this; }
const BSPBranch *BSPBranch::as_branch() const	{ return this; }
BSPLeaf *BSPBranch::as_leaf()					{ return NULL; }
const BSPLeaf *BSPBranch::as_leaf() const		{ return NULL; }
bool BSPBranch::is_leaf() const					{ return false; }
const BSPNode_Ptr& BSPBranch::left()			{ return m_left; }
BSPNode_CPtr BSPBranch::left() const			{ return m_left; }

void BSPBranch::output_postorder_text(std::ostream& os) const
{
	if(m_left.get()) m_left->output_postorder_text(os);
	if(m_right.get()) m_right->output_postorder_text(os);

	int parentIndex = m_parent != NULL ? m_parent->index() : -1;
	os << m_index << " B " << m_left->index() << ' ' << m_right->index() << ' ' << parentIndex;

	const Vector3d& n = m_splitter->normal();
	double d = m_splitter->distance_value();
	os << " ( " << n.x << ' ' << n.y << ' ' << n.z << ' ' << d << " )";

	os << '\n';
}

const BSPNode_Ptr& BSPBranch::right()			{ return m_right; }
BSPNode_CPtr BSPBranch::right() const			{ return m_right; }
Plane_CPtr BSPBranch::splitter() const			{ return m_splitter; }

}
