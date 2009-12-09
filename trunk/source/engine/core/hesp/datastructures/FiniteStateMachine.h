/***
 * hesperus: FiniteStateMachine.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_FINITESTATEMACHINE
#define H_HESP_FINITESTATEMACHINE

#include <map>
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

template <typename>
class FSMState
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::string m_name;

	//#################### CONSTRUCTORS ####################
public:
	explicit FSMState(const std::string& name)
	:	m_name(name)
	{}

	//#################### DESTRUCTOR ####################
public:
	virtual ~FSMState() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void enter() {}
	virtual void execute() = 0;
	virtual void leave() {}

	//#################### PUBLIC METHODS ####################
public:
	const std::string& name() const	{ return m_name; }
};

template <typename>
class FSMTransition
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::string m_name, m_from, m_to;

	//#################### CONSTRUCTORS ####################
public:
	FSMTransition(const std::string& name, const std::string& from, const std::string& to)
	:	m_name(name), m_from(from), m_to(to)
	{}

	//#################### DESTRUCTOR ####################
public:
	virtual ~FSMTransition() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual std::string execute() = 0;
	virtual bool triggered() const = 0;

	//#################### PUBLIC METHODS ####################
public:
	const std::string& from() const	{ return m_from; }
	const std::string& name() const	{ return m_name; }
	const std::string& to() const	{ return m_to; }
};

template <typename Dummy>
class FiniteStateMachine
{
	//#################### TYPEDEFS ####################
private:
	typedef FSMState<Dummy> State;
	typedef FSMTransition<Dummy> Transition;
public:
	typedef shared_ptr<State> State_Ptr;
	typedef shared_ptr<Transition> Transition_Ptr;
private:
	typedef std::map<std::string,State_Ptr> StateMap;
	typedef std::map<std::string,std::vector<Transition_Ptr> > TransitionMap;

	//#################### PRIVATE VARIABLES ####################
private:
	State_Ptr m_currentState;

	// Note:	These data structures will need to be changed if we ever want to support removal of states/transitions,
	//			but the class interface will stay the same. This is by design - I'm avoiding implementing the removal
	//			functions up-front because I may never actually need them.
	StateMap m_stateMap;
	TransitionMap m_transitionMap;

	//#################### PUBLIC METHODS ####################
public:
	void add_state(const State_Ptr& state);
	void add_transition(const Transition_Ptr& transition);
	bool execute();
#if 0
	void remove_state(const std::string& name);
	void remove_transition(const std::string& name);
#endif
	void set_initial_state(const std::string& name);
};

}

#include "FiniteStateMachine.tpp"

#endif
