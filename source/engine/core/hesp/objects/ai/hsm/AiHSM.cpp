/***
 * hesperus: AiHSM.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "AiHSM.h"

#include "AiHSMState.h"
#include "AiHSMTransition.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
AiHSM::AiHSM(const Database_Ptr& database)
:	m_database(database)
{}

//#################### PUBLIC METHODS ####################
void AiHSM::add_state(const AiHSMState_Ptr& state, const std::string& parent)
{
	state->set_database(m_database);
	m_base.add_state(state, parent);
}

void AiHSM::add_transition(const AiHSMTransition_Ptr& transition)
{
	transition->set_database(m_database);
	m_base.add_transition(transition);
}

AiBehaviour_Ptr AiHSM::execute()
{
	m_base.execute();
	const AiHSMState *activeDescendant = dynamic_cast<const AiHSMState*>(m_base.active_descendant());
	return activeDescendant->behaviour();
}

}
