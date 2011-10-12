/***
 * hesperus: AiCompositeBehaviour.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_AICOMPOSITEBEHAVIOUR
#define H_HESP_AICOMPOSITEBEHAVIOUR

#include "AiBehaviour.h"

namespace hesp {

class AiCompositeBehaviour : public AiBehaviour
{
	//#################### PROTECTED VARIABLES ####################
protected:
	std::vector<AiBehaviour_Ptr> m_children;

	//#################### PUBLIC METHODS ####################
public:
	void add_child(const AiBehaviour_Ptr& child);
};

}

#endif
