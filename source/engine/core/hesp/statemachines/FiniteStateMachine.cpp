/***
 * hesperus: FiniteStateMachine.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "FiniteStateMachine.h"

#include <hesp/exceptions/Exception.h>
#include "FSMState.h"
#include "FSMTransition.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
void FiniteStateMachine::add_state(const FSMState_Ptr& state)
{
	assert(state);
	if(!m_stateMap.insert(std::make_pair(state->name(), state)).second)
	{
		throw Exception("A state named " + state->name() + " already exists");
	}
}

void FiniteStateMachine::add_transition(const FSMTransition_Ptr& transition)
{
	assert(transition);
	m_transitionMap[transition->from()].push_back(transition);
}

/**
@return		true, if there has been a state transition during this execution, or false otherwise
*/
bool FiniteStateMachine::execute()
{
	// Check the current state's outgoing transitions, and change to a new state if necessary.
	TransitionMap::const_iterator it = m_transitionMap.find(m_currentState->name());
	if(it != m_transitionMap.end())
	{
		const std::vector<FSMTransition_Ptr>& transitions = it->second;
		for(size_t j=0, size=transitions.size(); j<size; ++j)
		{
			if(transitions[j]->triggered())
			{
				m_currentState->leave();
				transitions[j]->execute();

				std::string newState = transitions[j]->to();
				StateMap::const_iterator kt = m_stateMap.find(newState);
				if(kt != m_stateMap.end()) m_currentState = kt->second;
				else throw Exception("No such state: " + newState);

				m_currentState->enter();

				// If a transition's been triggered, we don't want to execute the new state yet,
				// as one of its transitions may be triggered as well. Rather than trying to
				// potentially follow an entire chain of triggered transitions in a single
				// execution of the FSM, we choose to only allow either a transition or an
				// execution of the current state each time.
				return true;
			}
		}
	}

	// If there was no transition above, execute the current state.
	m_currentState->execute();
	return false;
}

void FiniteStateMachine::set_initial_state(const std::string& name)
{
	if(!m_currentState)
	{
		StateMap::const_iterator it = m_stateMap.find(name);
		if(it != m_stateMap.end())
		{
			m_currentState = it->second;
			m_currentState->enter();
		}
		else throw Exception("No such state: " + name);
	}
	else throw Exception("Trying to set the initial state when another state is already current");
}

}
