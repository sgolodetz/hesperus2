/***
 * hesperus: InputBinding.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_INPUTBINDING
#define H_HESP_INPUTBINDING

#include <map>

#include "InputAction.h"
#include "Inputter.h"

namespace hesp {

class InputBinding
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::map<InputAction,Inputter_CPtr> m_inputters;

	//#################### CONSTRUCTORS ####################
public:
	explicit InputBinding(const std::map<InputAction,Inputter_CPtr>& inputters);

	//#################### PUBLIC OPERATORS ####################
public:
	Inputter_CPtr operator()(InputAction action) const;

	//#################### PUBLIC METHODS ####################
public:
	bool down(InputAction action, const InputState& input) const;
};

}

#endif
