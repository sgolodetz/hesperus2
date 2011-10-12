/***
 * hesperus: TreeSection.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_TREESECTION
#define H_HESP_TREESECTION

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class BSPTree> BSPTree_Ptr;
typedef shared_ptr<const class BSPTree> BSPTree_CPtr;

struct TreeSection
{
	//#################### LOADING METHODS ####################
	static BSPTree_Ptr load(std::istream& is);

	//#################### SAVING METHODS ####################
	static void save(std::ostream& os, const BSPTree_CPtr& tree);
};

}

#endif
