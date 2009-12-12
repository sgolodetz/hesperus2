/***
 * hesperus: OnionBranch.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "OnionBranch.h"

#include <hesp/math/geom/Plane.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
OnionBranch::OnionBranch(int index, const Plane_CPtr& splitter, const OnionNode_Ptr& left, const OnionNode_Ptr& right)
:	OnionNode(index), m_splitter(splitter), m_left(left), m_right(right)
{
	left->set_parent(this);
	right->set_parent(this);
}

//#################### PUBLIC METHODS ####################
OnionBranch *OnionBranch::as_branch()				{ return this; }
const OnionBranch *OnionBranch::as_branch() const	{ return this; }
OnionLeaf *OnionBranch::as_leaf()					{ return NULL; }
const OnionLeaf *OnionBranch::as_leaf() const		{ return NULL; }
bool OnionBranch::is_leaf() const					{ return false; }
const OnionNode_Ptr& OnionBranch::left()			{ return m_left; }
OnionNode_CPtr OnionBranch::left() const			{ return m_left; }

void OnionBranch::output_postorder_text(std::ostream& os) const
{
	if(m_left) m_left->output_postorder_text(os);
	if(m_right) m_right->output_postorder_text(os);

	int parentIndex = m_parent != NULL ? m_parent->index() : -1;
	os << m_index << " B " << m_left->index() << ' ' << m_right->index() << ' ' << parentIndex;

	const Vector3d& n = m_splitter->normal();
	double d = m_splitter->distance_value();
	os << " ( " << n.x << ' ' << n.y << ' ' << n.z << ' ' << d << " )";

	os << '\n';
}

const OnionNode_Ptr& OnionBranch::right()			{ return m_right; }
OnionNode_CPtr OnionBranch::right() const			{ return m_right; }
Plane_CPtr OnionBranch::splitter() const			{ return m_splitter; }

}
