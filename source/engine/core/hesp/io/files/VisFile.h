/***
 * hesperus: VisFile.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_VISFILE
#define H_HESP_VISFILE

#include <iosfwd>
#include <vector>

#include <hesp/vis/VisTable.h>

namespace hesp {

struct VisFile
{
	//#################### LOADING METHODS ####################
	static LeafVisTable_Ptr load(const std::string& filename);

	//#################### SAVING METHODS ####################
	static void save(const std::string& filename, const LeafVisTable_CPtr& leafVis);
};

}

#endif
