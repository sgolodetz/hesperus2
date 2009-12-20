/***
 * hesperus: HSMTransition.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_HSMTRANSITION
#define H_HESP_HSMTRANSITION

#include <string>

namespace hesp {

class HSMTransition
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::string m_name, m_from, m_to;

	//#################### CONSTRUCTORS ####################
public:
	HSMTransition(const std::string& name, const std::string& from, const std::string& to);

	//#################### DESTRUCTOR ####################
public:
	virtual ~HSMTransition();

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual bool triggered() const = 0;

	//#################### PUBLIC METHODS ####################
public:
	virtual void execute();
	const std::string& from() const;
	const std::string& name() const;
	const std::string& to() const;
};

}

#endif
