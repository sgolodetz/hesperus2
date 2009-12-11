/***
 * hesperus: FSMTransition.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "FSMTransition.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
FSMTransition::FSMTransition(const std::string& name, const std::string& from, const std::string& to)
:	m_name(name), m_from(from), m_to(to)
{}

//#################### DESTRUCTOR ####################
FSMTransition::~FSMTransition() {}

//#################### PUBLIC ABSTRACT METHODS ####################
void FSMTransition::execute()
{
	// Note:	A default implementation is provided so that transitions that don't
	//			need to do anything special when they're triggered aren't forced to
	//			provide an empty implementation.
}

//#################### PUBLIC METHODS ####################
const std::string& FSMTransition::from() const
{
	return m_from;
}

const std::string& FSMTransition::name() const
{
	return m_name;
}

const std::string& FSMTransition::to() const
{
	return m_to;
}

}
