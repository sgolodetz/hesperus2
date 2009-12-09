/***
 * test-fsm: main.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <iostream>

#include <hesp/datastructures/FiniteStateMachine.h>
#include <hesp/datastructures/FSMState.h>
#include <hesp/datastructures/FSMTransition.h>
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

typedef shared_ptr<GameData> GameData_Ptr;

//#################### STATES ####################

// Exit State

struct ExitState : FSMState
{
	GameData_Ptr m_sharedData;

	ExitState(const GameData_Ptr& sharedData)
	:	FSMState("Exit"), m_sharedData(sharedData)
	{}

	void enter()
	{
		std::cout << "Entering Exit\n";
	}

	void execute()
	{
		std::cout << "Executing Exit\n";
		m_sharedData->m_quit = true;
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

struct LevelState : FSMState
{
	GameData_Ptr m_sharedData;
	LevelData_Ptr m_localData;

	explicit LevelState(const GameData_Ptr& sharedData, const LevelData_Ptr& localData)
	:	FSMState("Level"), m_sharedData(sharedData), m_localData(localData)
	{}

	void enter()
	{
		std::cout << "Entering Level\n";
	}

	void execute()
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

struct MenuState : FSMState
{
	GameData_Ptr m_sharedData;
	MenuData_Ptr m_localData;

	explicit MenuState(const GameData_Ptr& sharedData, const MenuData_Ptr& localData)
	:	FSMState("Menu"), m_sharedData(sharedData), m_localData(localData)
	{}

	void enter()
	{
		std::cout << "Entering Menu\n";
	}

	void execute()
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
struct MenuToLevelTransition : FSMTransition
{
	GameData_Ptr m_sharedData;
	MenuData_Ptr m_localData;

	explicit MenuToLevelTransition(const std::string& name, const std::string& from, const std::string& to, const GameData_Ptr& sharedData, const MenuData_Ptr& localData)
	:	FSMTransition(name, from, to), m_sharedData(sharedData), m_localData(localData)
	{}

	void execute()
	{
		std::cout << "Executing MenuToLevelTransition\n";
	}

	bool triggered() const
	{
		return m_sharedData->m_skipMenu || m_localData->m_buttonPressed;
	}
};

struct LevelToExitTransition : FSMTransition
{
	GameData_Ptr m_sharedData;
	LevelData_Ptr m_localData;

	explicit LevelToExitTransition(const std::string& name, const std::string& from, const std::string& to, const GameData_Ptr& sharedData, const LevelData_Ptr& localData)
	:	FSMTransition(name, from, to), m_sharedData(sharedData), m_localData(localData)
	{}

	void execute()
	{
		std::cout << "Executing LevelToExitTransition\n";
	}

	bool triggered() const
	{
		return m_localData->m_frameCount == 3;
	}
};

int main()
{
	FiniteStateMachine fsm;

	// Add the states.
	shared_ptr<GameData> sharedData(new GameData(false));

	fsm.add_state(FSMState_Ptr(new ExitState(sharedData)));

	LevelData_Ptr levelData(new LevelData);
	fsm.add_state(FSMState_Ptr(new LevelState(sharedData, levelData)));

	MenuData_Ptr menuData(new MenuData);
	fsm.add_state(FSMState_Ptr(new MenuState(sharedData, menuData)));

	// Add the transitions.
	fsm.add_transition(FSMTransition_Ptr(new LevelToExitTransition("LevelToExit", "Level", "Exit", sharedData, levelData)));
	fsm.add_transition(FSMTransition_Ptr(new MenuToLevelTransition("MenuToLevel", "Menu", "Level", sharedData, menuData)));

	// Set the initial state.
	fsm.set_initial_state("Menu");

	// Run the FSM.
	while(!sharedData->m_quit)
	{
		fsm.execute();
	}

	return 0;
}
