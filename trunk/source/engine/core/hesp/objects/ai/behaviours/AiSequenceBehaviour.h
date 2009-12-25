/***
 * hesperus: AiSequenceBehaviour.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_AISEQUENCEBEHAVIOUR
#define H_HESP_AISEQUENCEBEHAVIOUR

#include "AiCompositeBehaviour.h"

namespace hesp {

class AiSequenceBehaviour : public AiCompositeBehaviour
{
	//#################### PRIVATE VARIABLES ####################
private:
	size_t m_current;
	Status m_status;

	//#################### CONSTRUCTORS ####################
public:
	AiSequenceBehaviour();

	//#################### PUBLIC METHODS ####################
public:
	std::vector<ObjectCommand_Ptr> generate_commands();
	Status status() const;
};

}

#endif
