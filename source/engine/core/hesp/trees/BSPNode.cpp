/***
 * hesperus: BSPNode.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "BSPNode.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
BSPNode::BSPNode(int index)
:	m_index(index), m_parent(NULL)
{}

//#################### PUBLIC METHODS ####################
int BSPNode::index() const					{ return m_index; }
const BSPNode *BSPNode::parent() const		{ return m_parent; }
void BSPNode::set_parent(BSPNode *parent)	{ m_parent = parent; }

}
