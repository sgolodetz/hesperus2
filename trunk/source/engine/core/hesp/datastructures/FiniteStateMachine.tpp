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
	// Check the current state's outgoing transitions, and change to a new state if necessary.
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

			// If a transition's been triggered, we don't want to execute the new state yet,
			// as one of its transitions may be triggered as well. Rather than trying to
			// potentially follow an entire chain of triggered transitions in a single
			// execution of the FSM, we choose to only allow either a transition or an
			// execution of the current state each time.
			return;
		}
	}

	// If there was no transition above, execute the current state.
	m_currentState->execute(m_sharedData);
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
