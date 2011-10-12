/***
 * hesperus: AiHSM.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_AIHSM
#define H_HESP_AIHSM

#include <hesp/statemachines/HierarchicalStateMachine.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class AiBehaviour> AiBehaviour_Ptr;
typedef shared_ptr<class AiHSMState> AiHSMState_Ptr;
typedef shared_ptr<class AiHSMTransition> AiHSMTransition_Ptr;
typedef shared_ptr<class Database> Database_Ptr;

class AiHSM
{
	//#################### PRIVATE VARIABLES ####################
private:
	HierarchicalStateMachine m_base;
	Database_Ptr m_database;

	//#################### CONSTRUCTORS ####################
public:
	explicit AiHSM(const Database_Ptr& database);

	//#################### PUBLIC METHODS ####################
public:
	void add_state(const AiHSMState_Ptr& state, const std::string& parent = "");
	void add_transition(const AiHSMTransition_Ptr& transition);
	AiBehaviour_Ptr execute();
};

}

#endif
