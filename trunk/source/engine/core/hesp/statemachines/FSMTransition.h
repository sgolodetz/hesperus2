/***
 * hesperus: FSMTransition.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_FSMTRANSITION
#define H_HESP_FSMTRANSITION

#include <string>

namespace hesp {

class FSMTransition
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::string m_name, m_from, m_to;

	//#################### CONSTRUCTORS ####################
public:
	FSMTransition(const std::string& name, const std::string& from, const std::string& to);

	//#################### DESTRUCTOR ####################
public:
	virtual ~FSMTransition();

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void execute();
	virtual bool triggered() const = 0;

	//#################### PUBLIC METHODS ####################
public:
	const std::string& from() const;
	const std::string& name() const;
	const std::string& to() const;
};

}

#endif
