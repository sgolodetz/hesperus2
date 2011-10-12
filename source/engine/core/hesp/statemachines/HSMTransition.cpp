/***
 * hesperus: HSMTransition.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "HSMTransition.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
HSMTransition::HSMTransition(const std::string& name, const std::string& from, const std::string& to)
:	m_name(name), m_from(from), m_to(to)
{}

//#################### DESTRUCTOR ####################
HSMTransition::~HSMTransition() {}

//#################### PUBLIC METHODS ####################
void HSMTransition::execute()
{
	// Default implementation so that subclasses aren't forced to implement it if they don't need to.
}

const std::string& HSMTransition::from() const	{ return m_from; }
const std::string& HSMTransition::name() const	{ return m_name; }
const std::string& HSMTransition::to() const	{ return m_to; }

}
