/***
 * hesperus: VisSection.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_VISSECTION
#define H_HESP_VISSECTION

#include <hesp/vis/VisTable.h>

namespace hesp {

struct VisSection
{
	//#################### LOADING METHODS ####################
	static LeafVisTable_Ptr load(std::istream& is);

	//#################### SAVING METHODS ####################
	static void save(std::ostream& os, const LeafVisTable_CPtr& leafVis);
};

}

#endif
