/***
 * hesperus: MouseButtonInputter.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MOUSEBUTTONINPUTTER
#define H_HESP_MOUSEBUTTONINPUTTER

#include "Inputter.h"
#include "MouseButton.h"

namespace hesp {

class MouseButtonInputter : public Inputter
{
	//#################### PRIVATE VARIABLES ####################
private:
	MouseButton m_button;

	//#################### CONSTRUCTORS ####################
public:
	explicit MouseButtonInputter(MouseButton button);

	//#################### PUBLIC METHODS ####################
public:
	bool down(const InputState& input) const;
	void release(InputState& input) const;
};

}

#endif
