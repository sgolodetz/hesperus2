/***
 * hesperus: OnionNode.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "OnionNode.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
OnionNode::OnionNode(int index)
:	m_index(index), m_parent(NULL)
{}

//#################### PUBLIC METHODS ####################
int OnionNode::index() const					{ return m_index; }
const OnionNode *OnionNode::parent() const		{ return m_parent; }
void OnionNode::set_parent(OnionNode *parent)	{ m_parent = parent; }

}
