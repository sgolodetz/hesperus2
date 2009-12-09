/***
 * hesperus: FSMState.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_FSMSTATE
#define H_HESP_FSMSTATE

#include <string>

namespace hesp {

class FSMState
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::string m_name;

	//#################### CONSTRUCTORS ####################
public:
	explicit FSMState(const std::string& name);

	//#################### DESTRUCTOR ####################
public:
	virtual ~FSMState();

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void enter();
	virtual void execute() = 0;
	virtual void leave();

	//#################### PUBLIC METHODS ####################
public:
	const std::string& name() const;
};

}

#endif
