/***
 * asx: ASXException.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ASXException.h"

//#################### CONSTRUCTORS ####################
ASXException::ASXException(const std::string& cause)
:	m_cause(cause)
{}

//#################### PUBLIC METHODS ####################
const std::string& ASXException::cause() const
{
	return m_cause;
}
