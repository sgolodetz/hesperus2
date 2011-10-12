/***
 * hesperus: AiHSMTransition.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_AIHSMTRANSITION
#define H_HESP_AIHSMTRANSITION

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <hesp/statemachines/HSMTransition.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class Database> Database_Ptr;

class AiHSMTransition : public HSMTransition
{
	//#################### FRIENDS ####################
	friend class AiHSM;

	//#################### PRIVATE VARIABLES ####################
private:
	Database_Ptr m_database;

	//#################### PROTECTED METHODS ####################
protected:
	const Database_Ptr& database();

	//#################### PRIVATE METHODS ####################
private:
	void set_database(const Database_Ptr& database);
};

}

#endif
