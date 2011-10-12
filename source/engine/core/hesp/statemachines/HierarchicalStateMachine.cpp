/***
 * hesperus: HierarchicalStateMachine.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "HierarchicalStateMachine.h"

#include <hesp/exceptions/Exception.h>
#include "HSMState.h"
#include "HSMTransition.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
HierarchicalStateMachine::HierarchicalStateMachine()
:	m_root(new HSMState("<Root>"))
{}

//#################### PUBLIC METHODS ####################
const HSMState *HierarchicalStateMachine::active_descendant() const
{
	HSMState *cur = m_root.get();
	while(cur->active_child()) cur = cur->active_child();
	return cur;
}

void HierarchicalStateMachine::add_state(const HSMState_Ptr& state, const std::string& parent)
{
	HSMState *parentState = (parent != "") ? lookup_state(parent) : m_root.get();
	bool succeeded = m_stateMap.insert(std::make_pair(state->name(), state)).second;
	if(succeeded) parentState->add_child(state.get());
	else throw Exception("A state named " + state->name() + " already exists");
}

void HierarchicalStateMachine::add_transition(const HSMTransition_Ptr& transition)
{
	m_transitionMap[transition->from()].push_back(transition);
}

/**
@return	true, if a transition occurred, or false otherwise
*/
bool HierarchicalStateMachine::execute()
{
	std::list<HSMState*> trail;		// stores the states visited whilst walking down from the root (in reverse order)

	HSMState *cur = m_root.get();

	// Walk down from the root of the tree to the lowest active state, looking for transitions as we go.
	do
	{
		trail.push_front(cur);

		// Check for transitions out of this state. Process the first one which triggers (if any).
		const std::vector<HSMTransition_Ptr>& transitions = m_transitionMap[cur->name()];
		for(size_t i=0, size=transitions.size(); i<size; ++i)
		{
			if(transitions[i]->triggered())
			{
				execute_transition_sequence(trail, transitions[i]);
				return true;
			}
		}

		// If no transitions triggered, carry on and process the active child of this state, if any.
		cur = cur->active_child();
	} while(cur);

	// The most recent state on the trail (which is non-empty) has no active child, so enter its
	// initial child (if any). If it's a leaf state (i.e. has no children), this has no effect.
	trail.front()->enter_initial_child();

	// Finally, execute all states on the trail in order (i.e. work back up towards the root).
	for(std::list<HSMState*>::const_iterator it=trail.begin(), iend=trail.end(); it!=iend; ++it)
	{
		(*it)->execute();
	}

	return false;
}

//#################### PRIVATE METHODS ####################
void HierarchicalStateMachine::execute_transition_sequence(std::list<HSMState*>& executeList,
														   const HSMTransition_Ptr& transition)
{
	HSMState *source = lookup_state(transition->from());
	HSMState *dest = lookup_state(transition->to());

	HSMState *sourceAncestor = source;
	HSMState *destAncestor = dest;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Step 1:	Find the common ancestor of the source and dest states, and build the relevant lists.
	//
	//			The leave list is constructed to be the list of states from the source to the common ancestor, not
	//			including the common ancestor itself.
	//
	//			The switch list is constructed to be the list of states from the common ancestor to the destination,
	//			again not including the common ancestor itself.
	//
	//			The execution list starts off as the list of states from the source to the root, and ends up as the
	//			list of states which need executing after the transition has finished (i.e. the states above the
	//			point where the source and destination branches meet).
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	std::list<HSMState*> leaveList;
	std::list<HSMState*> switchList;

	// First get them to the same level by walking the lower of the two up the tree. (Only one of these loops will run.)
	while(sourceAncestor->level() > destAncestor->level())
	{
		leaveList.push_back(sourceAncestor);
		executeList.pop_front();
		sourceAncestor = sourceAncestor->parent();
	}

	while(destAncestor->level() > sourceAncestor->level())
	{
		switchList.push_front(destAncestor);
		destAncestor = destAncestor->parent();
	}

	// Now walk them both up the tree in sync until they hit each other.
	while(sourceAncestor != destAncestor)
	{
		leaveList.push_back(sourceAncestor);
		executeList.pop_front();
		switchList.push_front(destAncestor);

		sourceAncestor = sourceAncestor->parent();
		destAncestor = destAncestor->parent();
	}

	if(source == destAncestor || dest == sourceAncestor)
	{
		// TODO: This should ideally be checked when adding the transition in the first place.
		throw Exception("Transitions from descendant to ancestor, or vice-versa, are not allowed");
	}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Step 2: Run the relevant state actions in order.
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	// Call leave_active_child() on all states between the source node and the common ancestor (non-inclusive).
	// Note that we don't call leave_active_child() on the source node, because we're leaving it, not its child.
	// We don't call leave_active_child() on the common ancestor because leaving its child will be taken care of
	// when its active child is switched.
	leaveList.pop_front();
	for(std::list<HSMState*>::const_iterator it=leaveList.begin(), iend=leaveList.end(); it!=iend; ++it)
	{
		(*it)->leave_active_child();
	}

	// Switch the active child for the common ancestor (running the transition in the process).
	// Note that the child to which to switch is on the front of the switch list at this point.
	sourceAncestor->switch_active_child(switchList.front(), transition);

	// Run switch_active_child() on all states between the common ancestor and the dest node (non-inclusive).
	// Note that the switch list ultimately runs from the grandchild of the common ancestor to the common
	// ancestor, but we call switch_active_child() on the parent of the state in the switch list in each
	// case. Doing it this way makes the loop simpler.
	switchList.pop_front();
	for(std::list<HSMState*>::const_iterator it=switchList.begin(), iend=switchList.end(); it!=iend; ++it)
	{
		(*it)->parent()->switch_active_child(*it);
	}

	// Execute any states above the common ancestor.
	for(std::list<HSMState*>::const_iterator it=executeList.begin(), iend=executeList.end(); it!=iend; ++it)
	{
		(*it)->execute();
	}
}

HSMState *HierarchicalStateMachine::lookup_state(const std::string& name)
{
	StateMap::const_iterator it = m_stateMap.find(name);
	if(it != m_stateMap.end()) return it->second.get();
	else throw Exception("No such state: " + name);
}

}
