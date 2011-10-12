/***
 * test-hsm: main.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <iostream>

#include <hesp/statemachines/HierarchicalStateMachine.h>
#include <hesp/statemachines/HSMState.h>
#include <hesp/statemachines/HSMTransition.h>
using namespace hesp;

class TestState : public HSMState
{
	//#################### CONSTRUCTORS ####################
public:
	TestState(const std::string& name) : HSMState(name) {}

	//#################### PUBLIC METHODS ####################
public:
	void enter()	{ std::cout << "Enter " << name() << '\n'; }
	void execute()	{ std::cout << "Execute " << name() << '\n'; }
	void leave()	{ std::cout << "Leave " << name() << '\n'; }
};

class TestTransition : public HSMTransition
{
	//#################### PRIVATE VARIABLES ####################
private:
	mutable bool m_triggered;

	//#################### CONSTRUCTORS ####################
public:
	TestTransition(const std::string& name, const std::string& from, const std::string& to)
	:	HSMTransition(name, from, to), m_triggered(false)
	{}

	//#################### PUBLIC METHODS ####################
public:
	void execute()	{ std::cout << "Transition " << name() << '\n'; }
	void trigger()	{ m_triggered = true; }

	bool triggered() const
	{
		bool ret = m_triggered;
		m_triggered = false;
		return ret;
	}
};

typedef shared_ptr<TestTransition> TestTransition_Ptr;

void execute(HierarchicalStateMachine& hsm)
{
	hsm.execute();
	std::cout << "In State: " << ((TestState*)hsm.active_descendant())->name() << '\n';
	std::cout << '\n';
}

int main()
{
	// Note: This test program is loosely based on the HSM in Artifical Intelligence for Games (p.324).

	// Construct the state machine.
	HierarchicalStateMachine hsm;

	// Add the states.
	hsm.add_state(HSMState_Ptr(new TestState("L")));
	hsm.add_state(HSMState_Ptr(new TestState("M")));
	hsm.add_state(HSMState_Ptr(new TestState("N")));
	hsm.add_state(HSMState_Ptr(new TestState("A")), "L");
	hsm.add_state(HSMState_Ptr(new TestState("B")), "L");
	hsm.add_state(HSMState_Ptr(new TestState("C")), "L");

	// Extra states needed for my test.
	hsm.add_state(HSMState_Ptr(new TestState("P")));
	hsm.add_state(HSMState_Ptr(new TestState("D")), "P");
	hsm.add_state(HSMState_Ptr(new TestState("E")), "P");

	// Add the transitions.
	TestTransition_Ptr T1(new TestTransition("1", "A", "B"));	hsm.add_transition(T1);
	TestTransition_Ptr T2(new TestTransition("2", "M", "C"));	hsm.add_transition(T2);
	TestTransition_Ptr T3(new TestTransition("3", "B", "N"));	hsm.add_transition(T3);
	TestTransition_Ptr T4(new TestTransition("4", "L", "M"));	hsm.add_transition(T4);
	TestTransition_Ptr T5(new TestTransition("5", "M", "N"));	hsm.add_transition(T5);
	TestTransition_Ptr T6(new TestTransition("6", "N", "L"));	hsm.add_transition(T6);
	TestTransition_Ptr T7(new TestTransition("7", "N", "M"));	hsm.add_transition(T7);

	// Extra transitions needed for my test.
	TestTransition_Ptr T8(new TestTransition("8", "L", "D"));	hsm.add_transition(T8);
	TestTransition_Ptr T9(new TestTransition("9", "D", "E"));	hsm.add_transition(T9);
	TestTransition_Ptr T10(new TestTransition("10", "E", "B"));	hsm.add_transition(T10);

#if 0
	// Test from the book.
	execute(hsm);
	execute(hsm);

	T1->trigger();
	execute(hsm);

	T4->trigger();
	execute(hsm);

	T5->trigger();
	execute(hsm);

	T6->trigger();
	execute(hsm);

	T3->trigger();
	execute(hsm);

	T7->trigger();
	execute(hsm);

	T2->trigger();
	execute(hsm);
#else
	// My test.
	execute(hsm);
	execute(hsm);

	T8->trigger();
	execute(hsm);

	T9->trigger();
	execute(hsm);

	T10->trigger();
	execute(hsm);
#endif

	return 0;
}
