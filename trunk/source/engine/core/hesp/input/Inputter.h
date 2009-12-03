/***
 * hesperus: Inputter.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_INPUTTER
#define H_HESP_INPUTTER

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
class InputState;

class Inputter
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~Inputter() {}

	//#################### PUBLIC METHODS ####################
public:
	virtual bool down(const InputState& input) const = 0;
	virtual void release(InputState& input) const = 0;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<const Inputter> Inputter_CPtr;

}

#endif
