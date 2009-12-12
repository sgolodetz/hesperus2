/***
 * hesperus: OnionTreeSection.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "OnionTreeSection.h"

#include <hesp/io/util/LineIO.h>
#include <hesp/trees/OnionTree.h>

namespace hesp {

//#################### LOADING METHODS ####################
/**
Loads an onion tree from the specified std::istream.

@param is	The std::istream
@return		The onion tree
*/
OnionTree_Ptr OnionTreeSection::load(std::istream& is)
{
	LineIO::read_checked_line(is, "OnionTree");
	LineIO::read_checked_line(is, "{");
	OnionTree_Ptr tree = OnionTree::load_postorder_text(is);
	LineIO::read_checked_line(is, "}");
	return tree;
}

//#################### SAVING METHODS ####################
/**
Saves an onion tree to the specified std::ostream.

@param os		The std::ostream
@param tree		The onion tree
*/
void OnionTreeSection::save(std::ostream& os, const OnionTree_CPtr& tree)
{
	os << "OnionTree\n";
	os << "{\n";
	tree->output_postorder_text(os);
	os << "}\n";
}

}
