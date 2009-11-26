/***
 * hesperus: InvalidParameterException.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_INVALIDPARAMETEREXCEPTION
#define H_HESP_INVALIDPARAMETEREXCEPTION

#include "Exception.h"

namespace hesp {

class InvalidParameterException : public Exception
{
	//#################### CONSTRUCTORS ####################
public:
	explicit InvalidParameterException(const std::string& message)
	:	Exception("Invalid Parameter: " + message)
	{}
};

}

#endif
