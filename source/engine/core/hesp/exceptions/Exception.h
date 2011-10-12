/***
 * hesperus: Exception.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_EXCEPTION
#define H_HESP_EXCEPTION

#include <string>

namespace hesp {

class Exception
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::string m_cause;

	//#################### CONSTRUCTORS ####################
public:
	explicit Exception(const std::string& cause)
	:	m_cause(cause)
	{}

	//#################### DESTRUCTOR ####################
public:
	virtual ~Exception() {}

	//#################### PUBLIC METHODS ####################
	virtual const std::string& cause() const
	{
		return m_cause;
	}
};

}

#endif
