/***
 * ScarletPimpernel: GameFSM.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_SCARLETPIMPERNEL_GAMEFSM
#define H_SCARLETPIMPERNEL_GAMEFSM

#include <hesp/datastructures/FiniteStateMachine.h>
#include "GameData.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class GUIComponent> GUIComponent_Ptr;

//#################### MAIN STUFF ####################
typedef FiniteStateMachine<GameData> GameFSM;

class GameFSMState : public FSMState<GameData>
{
	//#################### CONSTRUCTORS ####################
public:
	explicit GameFSMState(const std::string& name);

	//#################### PROTECTED METHODS ####################
	void set_display(const GUIComponent_Ptr& display);
};

typedef FSMTransition<GameData> GameFSMTransition;

typedef shared_ptr<GameFSM> GameFSM_Ptr;
typedef shared_ptr<GameFSMState> GameFSMState_Ptr;
typedef shared_ptr<GameFSMTransition> GameFSMTransition_Ptr;

}

#endif
