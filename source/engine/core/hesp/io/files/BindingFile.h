/***
 * hesperus: BindingFile.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_BINDINGFILE
#define H_HESP_BINDINGFILE

#include <string>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<const class InputBinding> InputBinding_CPtr;

class BindingFile
{
	//#################### LOADING METHODS ####################
public:
	static InputBinding_CPtr load(const std::string& filename);
};

}

#endif
