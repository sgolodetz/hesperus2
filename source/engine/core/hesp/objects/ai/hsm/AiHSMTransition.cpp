/***
 * hesperus: AiHSMTransition.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "AiHSMTransition.h"

namespace hesp {

//#################### PROTECTED METHODS ####################
const Database_Ptr& AiHSMTransition::database()
{
	return m_database;
}

//#################### PRIVATE METHODS ####################
void AiHSMTransition::set_database(const Database_Ptr& database)
{
	m_database = database;
}

}
