/***
 * hesperus: AiHSMState.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "AiHSMState.h"

namespace hesp {

//#################### PROTECTED METHODS ####################
const Database_Ptr& AiHSMState::database()
{
	return m_database;
}

//#################### PRIVATE METHODS ####################
void AiHSMState::set_database(const Database_Ptr& database)
{
	m_database = database;
}

}
