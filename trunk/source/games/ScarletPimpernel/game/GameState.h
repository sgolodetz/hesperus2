/***
 * hesperus: GameState.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_GAMESTATE
#define H_HESP_GAMESTATE

#include <string>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <hesp/datastructures/FSMState.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class GUIComponent> GUIComponent_Ptr;

class GameState : public FSMState
{
	//#################### CONSTRUCTORS ####################
public:
	explicit GameState(const std::string& name);

	//#################### PROTECTED METHODS ####################
protected:
	void set_display(const GUIComponent_Ptr& display);
};

}

#endif
