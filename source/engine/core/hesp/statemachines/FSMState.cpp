/***
 * hesperus: FSMState.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "FSMState.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
FSMState::FSMState(const std::string& name)
:	m_name(name)
{}

//#################### DESTRUCTOR ####################
FSMState::~FSMState() {}

//#################### PUBLIC ABSTRACT METHODS ####################
void FSMState::enter() {}
void FSMState::leave() {}

//#################### PUBLIC METHODS ####################
const std::string& FSMState::name() const
{
	return m_name;
}

}
