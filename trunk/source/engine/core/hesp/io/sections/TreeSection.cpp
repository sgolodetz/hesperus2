/***
 * hesperus: TreeSection.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "TreeSection.h"

#include <iostream>

#include <hesp/io/util/LineIO.h>
#include <hesp/level/trees/BSPTree.h>

namespace hesp {

//#################### LOADING METHODS ####################
/**
Loads a BSP tree from the specified std::istream.

@param is	The std::istream
@return		The BSP tree
*/
BSPTree_Ptr TreeSection::load(std::istream& is)
{
	std::string line;
	LineIO::read_checked_line(is, "BSPTree");
	LineIO::read_checked_line(is, "{");
	BSPTree_Ptr tree = BSPTree::load_postorder_text(is);
	LineIO::read_checked_line(is, "}");
	return tree;
}

//#################### SAVING METHODS ####################
/**
Saves a BSP tree to the specified std::ostream.

@param os		The std::ostream
@param tree		The BSP tree
*/
void TreeSection::save(std::ostream& os, const BSPTree_CPtr& tree)
{
	os << "BSPTree\n";
	os << "{\n";
	tree->output_postorder_text(os);
	os << "}\n";
}


}
