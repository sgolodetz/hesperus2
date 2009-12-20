/***
 * hesperus: FiniteStateMachine.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_FINITESTATEMACHINE
#define H_HESP_FINITESTATEMACHINE

#include <map>
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class FSMState> FSMState_Ptr;
typedef shared_ptr<class FSMTransition> FSMTransition_Ptr;

class FiniteStateMachine
{
	//#################### TYPEDEFS ####################
private:
	typedef std::map<std::string,FSMState_Ptr> StateMap;
	typedef std::map<std::string,std::vector<FSMTransition_Ptr> > TransitionMap;

	//#################### PRIVATE VARIABLES ####################
private:
	FSMState_Ptr m_currentState;
	StateMap m_stateMap;
	TransitionMap m_transitionMap;

	//#################### PUBLIC METHODS ####################
public:
	void add_state(const FSMState_Ptr& state);
	void add_transition(const FSMTransition_Ptr& transition);
	bool execute();
	void set_initial_state(const std::string& name);
};

//#################### TYPEDEFS ####################
typedef shared_ptr<FiniteStateMachine> FiniteStateMachine_Ptr;

}

#endif
