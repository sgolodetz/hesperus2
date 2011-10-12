/***
 * hesperus: HSMState.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_HSMSTATE
#define H_HESP_HSMSTATE

#include <string>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class HSMTransition> HSMTransition_Ptr;

class HSMState
{
	//#################### FRIENDS ####################
	friend class HierarchicalStateMachine;

	//#################### PRIVATE VARIABLES ####################
private:
	HSMState *m_activeChild;
	HSMState *m_initialChild;
	int m_level;
	std::string m_name;
	HSMState *m_parent;

	//#################### CONSTRUCTORS ####################
public:
	explicit HSMState(const std::string& name);

	//#################### DESTRUCTOR ####################
public:
	virtual ~HSMState();

	//#################### PUBLIC METHODS ####################
public:
	virtual void enter();
	virtual void execute();
	virtual void leave();
	const std::string& name() const;

	//#################### PRIVATE METHODS ####################
private:
	HSMState *active_child();
	void add_child(HSMState *child);
	void enter_initial_child();
	void leave_active_child();
	int level() const;
	HSMState *parent();
	void switch_active_child(HSMState *newActiveChild, const HSMTransition_Ptr& transition = HSMTransition_Ptr());
};

}

#endif
