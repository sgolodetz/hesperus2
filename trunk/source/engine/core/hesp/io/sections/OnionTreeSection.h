/***
 * hesperus: OnionTreeSection.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ONIONTREESECTION
#define H_HESP_ONIONTREESECTION

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class OnionTree> OnionTree_Ptr;
typedef shared_ptr<const class OnionTree> OnionTree_CPtr;

struct OnionTreeSection
{
	//#################### LOADING METHODS ####################
	static OnionTree_Ptr load(std::istream& is);

	//#################### SAVING METHODS ####################
	static void save(std::ostream& os, const OnionTree_CPtr& tree);
};

}

#endif
