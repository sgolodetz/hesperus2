/***
 * hesperus: NavFile.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_NAVFILE
#define H_HESP_NAVFILE

#include <string>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class NavManager> NavManager_Ptr;
typedef shared_ptr<const class NavManager> NavManager_CPtr;

struct NavFile
{
	//#################### LOADING METHODS ####################
	static NavManager_Ptr load(const std::string& filename);

	//#################### SAVING METHODS ####################
	static void save(const std::string& filename, const NavManager_CPtr& navManager);
};

}

#endif
