/***
 * hesperus: AdjacencyTable.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ADJACENCYTABLE
#define H_HESP_ADJACENCYTABLE

#include <vector>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
class AdjacencyList;

/**
This class provides an adjacency table representation of a +ve-weighted,
directed graph.
*/
class AdjacencyTable
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::vector<std::vector<float> > m_table;
	int m_size;

	//#################### CONSTRUCTORS ####################
public:
	AdjacencyTable(const AdjacencyList& adjList);

	//#################### PUBLIC OPERATORS ####################
public:
	float operator()(int i, int j) const;

	//#################### PUBLIC METHODS ####################
public:
	int size() const;
};

}

#endif
