/***
 * hesperus: AiCompositeBehaviour.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "AiCompositeBehaviour.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
void AiCompositeBehaviour::add_child(const AiBehaviour_Ptr& child)
{
	m_children.push_back(child);
}

}
