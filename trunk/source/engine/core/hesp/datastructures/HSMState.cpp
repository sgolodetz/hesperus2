/***
 * hesperus: HSMState.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "HSMState.h"

#include "HSMTransition.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
HSMState::HSMState(const std::string& name)
:	m_activeChild(NULL), m_initialChild(NULL), m_level(0), m_name(name), m_parent(NULL)
{}

//#################### DESTRUCTOR ####################
HSMState::~HSMState() {}

//#################### PUBLIC METHODS ####################
void HSMState::enter()
{
	// Default implementation so that subclasses aren't forced to implement it if they don't need to.
}

void HSMState::execute()
{
	// Default implementation so that subclasses aren't forced to implement it if they don't need to.
}

void HSMState::leave()
{
	// Default implementation so that subclasses aren't forced to implement it if they don't need to.
}

const std::string& HSMState::name() const
{
	return m_name;
}

//#################### PRIVATE METHODS ####################
HSMState *HSMState::active_child()
{
	return m_activeChild;
}

void HSMState::add_child(HSMState *child)
{
	child->m_level = m_level + 1;
	child->m_parent = this;
	if(!m_initialChild) m_initialChild = child;
}

void HSMState::enter_initial_child()
{
	assert(!m_activeChild);
	if(m_initialChild)
	{
		m_activeChild = m_initialChild;
		m_activeChild->enter();
	}
}

void HSMState::leave_active_child()
{
	if(m_activeChild)
	{
		m_activeChild->leave();
		m_activeChild = NULL;
	}
}

int HSMState::level() const
{
	return m_level;
}

HSMState *HSMState::parent()
{
	return m_parent;
}

void HSMState::switch_active_child(HSMState *newActiveChild, const HSMTransition_Ptr& transition)
{
	assert(newActiveChild != NULL);
	if(newActiveChild != m_activeChild)
	{
		if(m_activeChild) m_activeChild->leave();
		if(transition) transition->execute();
		m_activeChild = newActiveChild;
		m_activeChild->enter();
	}
}

}
