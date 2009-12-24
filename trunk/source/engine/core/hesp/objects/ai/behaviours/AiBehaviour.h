/***
 * hesperus: AiBehaviour.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_AIBEHAVIOUR
#define H_HESP_AIBEHAVIOUR

#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class ObjectCommand> ObjectCommand_Ptr;

class AiBehaviour
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~AiBehaviour();

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual std::vector<ObjectCommand_Ptr> generate_commands() = 0;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<AiBehaviour> AiBehaviour_Ptr;
typedef shared_ptr<const AiBehaviour> AiBehaviour_CPtr;

}

#endif
