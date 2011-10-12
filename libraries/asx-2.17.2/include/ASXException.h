/***
 * asx: ASXException.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_ASX_ASXEXCEPTION
#define H_ASX_ASXEXCEPTION

#include <string>

class ASXException
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::string m_cause;

	//#################### CONSTRUCTORS ####################
public:
	explicit ASXException(const std::string& cause);

	//#################### PUBLIC METHODS ####################
public:
	const std::string& cause() const;
};

#endif
