/***
 * hesperus: AiHSMState.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_AIHSMSTATE
#define H_HESP_AIHSMSTATE

#include <hesp/statemachines/HSMState.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class AiBehaviour> AiBehaviour_Ptr;
typedef shared_ptr<class Database> Database_Ptr;

class AiHSMState : public HSMState
{
	//#################### FRIENDS ####################
	friend class AiHSM;

	//#################### PRIVATE VARIABLES ####################
private:
	Database_Ptr m_database;

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual AiBehaviour_Ptr behaviour() const = 0;

	//#################### PROTECTED METHODS ####################
protected:
	const Database_Ptr& database();

	//#################### PRIVATE METHODS ####################
private:
	void set_database(const Database_Ptr& database);
};

}

#endif
