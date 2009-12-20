/***
 * hesperus: HierarchicalStateMachine.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_HIERARCHICALSTATEMACHINE
#define H_HESP_HIERARCHICALSTATEMACHINE

#include <list>
#include <map>
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class HSMState> HSMState_Ptr;
typedef shared_ptr<class HSMTransition> HSMTransition_Ptr;

class HierarchicalStateMachine
{
	//#################### TYPEDEFS ####################
private:
	typedef std::map<std::string,HSMState_Ptr> StateMap;
	typedef std::map<std::string,std::vector<HSMTransition_Ptr> > TransitionMap;

	//#################### PRIVATE VARIABLES ####################
private:
	HSMState_Ptr m_root;
	StateMap m_stateMap;
	TransitionMap m_transitionMap;

	//#################### CONSTRUCTORS ####################
public:
	HierarchicalStateMachine();

	//#################### PUBLIC METHODS ####################
public:
	const HSMState *active_descendant() const;
	void add_state(const HSMState_Ptr& state, const std::string& parent = "");
	void add_transition(const HSMTransition_Ptr& transition);
	bool execute();

	//#################### PRIVATE METHODS ####################
private:
	void execute_transition_sequence(std::list<HSMState*>& executeList, const HSMTransition_Ptr& transition);
	HSMState *lookup_state(const std::string& name);
};

}

#endif
