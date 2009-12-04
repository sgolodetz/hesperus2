/***
 * hesperus: GameState.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_GAMESTATE
#define H_HESP_GAMESTATE

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class GUIComponent> GUIComponent_Ptr;
class InputState;

//#################### TYPEDEFS ####################
typedef shared_ptr<class GameState> GameState_Ptr;

class GameState
{
	//#################### PRIVATE VARIABLES ####################
private:
	bool m_quitFlag;

	//#################### CONSTRUCTORS ####################
public:
	GameState();

	//#################### DESTRUCTOR ####################
public:
	virtual ~GameState();

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void enter() = 0;
	virtual void leave() = 0;
	virtual GameState_Ptr update(int milliseconds, InputState& input) = 0;

	//#################### PUBLIC METHODS ####################
public:
	bool quit_requested() const;

	//#################### PROTECTED METHODS ####################
protected:
	void set_display(const GUIComponent_Ptr& display);
	void set_quit_flag();
};

}

#endif
