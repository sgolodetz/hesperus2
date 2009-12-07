/***
 * hesperus: FiniteStateMachine.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <cassert>

#include <hesp/exceptions/Exception.h>

#define FSM_HEADER	template <typename SharedData>
#define FSM_THIS	FiniteStateMachine<SharedData>

namespace hesp {

//#################### CONSTRUCTORS ####################
FSM_HEADER
FSM_THIS::FiniteStateMachine(const shared_ptr<SharedData>& sharedData)
:	m_sharedData(sharedData)
{}

//#################### PUBLIC METHODS ####################
FSM_HEADER
void FSM_THIS::add_state(const State_Ptr& state)
{
	assert(state);
	if(!m_stateMap.insert(std::make_pair(state->name(), state)).second)
	{
		throw Exception("A state named " + state->name() + " already exists");
	}
}

FSM_HEADER
void FSM_THIS::add_transition(const Transition_Ptr& transition)
{
	assert(transition);
	m_transitionMap[transition->from()].push_back(transition);
}

FSM_HEADER
void FSM_THIS::execute()
{
	// Execute the current state.
	m_currentState->execute(m_sharedData);

	// Check its outgoing transitions.
	typename TransitionMap::const_iterator it = m_transitionMap.find(m_currentState->name());
	if(it == m_transitionMap.end()) return;

	const std::vector<Transition_Ptr>& transitions = it->second;
	for(size_t j=0, size=transitions.size(); j<size; ++j)
	{
		if(transitions[j]->triggered(m_sharedData))
		{
			m_currentState->leave();

			std::string newState = transitions[j]->execute(m_sharedData);
			typename StateMap::const_iterator kt = m_stateMap.find(newState);
			if(kt != m_stateMap.end()) m_currentState = kt->second;
			else throw Exception("No such state: " + newState);

			m_currentState->enter();

			break;
		}
	}
}

FSM_HEADER
void FSM_THIS::set_initial_state(const std::string& name)
{
	if(!m_currentState)
	{
		typename StateMap::const_iterator it = m_stateMap.find(name);
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

#undef FSM_HEADER
#undef FSM_THIS
