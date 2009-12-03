/***
 * hesperus: KeyInputter.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_KEYINPUTTER
#define H_HESP_KEYINPUTTER

#include <SDL_keysym.h>

#include "Inputter.h"

namespace hesp {

class KeyInputter : public Inputter
{
	//#################### PRIVATE VARIABLES ####################
private:
	SDLKey m_key;

	//#################### CONSTRUCTORS ####################
public:
	explicit KeyInputter(SDLKey key);

	//#################### PUBLIC METHODS ####################
public:
	bool down(const InputState& input) const;
	void release(InputState& input) const;
};

}

#endif
