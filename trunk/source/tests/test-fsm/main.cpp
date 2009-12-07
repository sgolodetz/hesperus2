/***
 * test-fsm: main.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <iostream>

#include <hesp/datastructures/FiniteStateMachine.h>
using namespace hesp;

//#################### SHARED DATA ####################
struct GameData
{
	bool m_skipMenu;
	bool m_quit;

	explicit GameData(bool skipMenu)
	:	m_skipMenu(skipMenu), m_quit(false)
	{}
};

//#################### STATES ####################

// Exit State

struct ExitState : FSMState<GameData>
{
	ExitState()
	:	FSMState<GameData>("Exit")
	{}

	void enter()
	{
		std::cout << "Entering Exit\n";
	}

	void execute(const shared_ptr<GameData>& sharedData)
	{
		std::cout << "Executing Exit\n";
		sharedData->m_quit = true;
	}

	void leave()
	{
		std::cout << "Leaving Exit\n";
	}
};

// Level State

struct LevelData
{
	int m_frameCount;

	LevelData()
	:	m_frameCount(0)
	{}
};

typedef shared_ptr<LevelData> LevelData_Ptr;

struct LevelState : FSMState<GameData>
{
	LevelData_Ptr m_localData;

	explicit LevelState(const LevelData_Ptr& localData)
	:	FSMState<GameData>("Level"), m_localData(localData)
	{}

	void enter()
	{
		std::cout << "Entering Level\n";
	}

	void execute(const shared_ptr<GameData>&)
	{
		std::cout << "Executing Level\n";
		++m_localData->m_frameCount;
	}

	void leave()
	{
		std::cout << "Leaving Level\n";
	}
};

// Menu State

struct MenuData
{
	bool m_buttonPressed;

	MenuData()
	:	m_buttonPressed(false)
	{}
};

typedef shared_ptr<MenuData> MenuData_Ptr;

struct MenuState : FSMState<GameData>
{
	MenuData_Ptr m_localData;

	explicit MenuState(const MenuData_Ptr& localData)
	:	FSMState<GameData>("Menu"), m_localData(localData)
	{}

	void enter()
	{
		std::cout << "Entering Menu\n";
	}

	void execute(const shared_ptr<GameData>&)
	{
		std::cout << "Executing Menu\n";
		m_localData->m_buttonPressed = true;
	}

	void leave()
	{
		std::cout << "Leaving Menu\n";
	}
};

//#################### TRANSITIONS ####################
struct MenuToLevelTransition : FSMTransition<GameData>
{
	MenuData_Ptr m_localData;

	explicit MenuToLevelTransition(const std::string& name, const std::string& from, const std::string& to, const MenuData_Ptr& localData)
	:	FSMTransition<GameData>(name, from, to), m_localData(localData)
	{}

	std::string execute(const shared_ptr<GameData>&)
	{
		std::cout << "Executing MenuToLevelTransition\n";
		return "Level";
	}

	bool triggered(const shared_ptr<const GameData>& sharedData) const
	{
		return sharedData->m_skipMenu || m_localData->m_buttonPressed;
	}
};

struct LevelToExitTransition : FSMTransition<GameData>
{
	LevelData_Ptr m_localData;

	explicit LevelToExitTransition(const std::string& name, const std::string& from, const std::string& to, const LevelData_Ptr& localData)
	:	FSMTransition<GameData>(name, from, to), m_localData(localData)
	{}

	std::string execute(const shared_ptr<GameData>&)
	{
		std::cout << "Executing LevelToExitTransition\n";
		return "Exit";
	}

	bool triggered(const shared_ptr<const GameData>&) const
	{
		return m_localData->m_frameCount == 3;
	}
};

int main()
{
	typedef FiniteStateMachine<GameData> GameFSM;
	shared_ptr<GameData> sharedData(new GameData(false));
	GameFSM fsm(sharedData);

	// Add the states.
	fsm.add_state(GameFSM::State_Ptr(new ExitState));

	LevelData_Ptr levelData(new LevelData);
	fsm.add_state(GameFSM::State_Ptr(new LevelState(levelData)));

	MenuData_Ptr menuData(new MenuData);
	fsm.add_state(GameFSM::State_Ptr(new MenuState(menuData)));

	// Add the transitions.
	fsm.add_transition(GameFSM::Transition_Ptr(new LevelToExitTransition("LevelToExit", "Level", "Exit", levelData)));
	fsm.add_transition(GameFSM::Transition_Ptr(new MenuToLevelTransition("MenuToLevel", "Menu", "Level", menuData)));

	// Set the initial state.
	fsm.set_initial_state("Menu");

	// Run the FSM.
	while(!sharedData->m_quit)
	{
		fsm.execute();
	}

	return 0;
}
