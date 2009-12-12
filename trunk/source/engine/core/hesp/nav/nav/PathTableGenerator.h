/***
 * hesperus: PathTableGenerator.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_PATHTABLEGENERATOR
#define H_HESP_PATHTABLEGENERATOR

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
class AdjacencyTable;
typedef shared_ptr<class PathTable> PathTable_Ptr;

struct PathTableGenerator
{
	//#################### PUBLIC METHODS ####################
	static PathTable_Ptr floyd_warshall(const AdjacencyTable& adjTable);
};

}

#endif
