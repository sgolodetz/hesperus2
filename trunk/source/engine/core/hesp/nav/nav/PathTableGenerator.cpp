/***
 * hesperus: PathTableGenerator.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "PathTableGenerator.h"

#include <algorithm>

#include "AdjacencyTable.h"
#include "PathTable.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
PathTable_Ptr PathTableGenerator::floyd_warshall(const AdjacencyTable& adjTable)
{
	// Reference: See p.558-62 of Introduction to Algorithms (Cormen, Leiserson and Rivest) 1st Ed.

	int size = adjTable.size();
	PathTable_Ptr pathTable(new PathTable(size));

	/*
	Initialise the path table from the adjacency (weight) table.

	Note that the base formula for the next nodes (sigma values) is given by:

	sigma_{ij}^0 =	{ -1	if i = j or w_{ij} = inf
					{ j		otherwise
	*/
	for(int i=0; i<size; ++i)
		for(int j=0; j<size; ++j)
		{
			if(i != j && adjTable(i,j) != INT_MAX)
			{
				pathTable->cost(i,j) = adjTable(i,j);
				pathTable->next_node(i,j) = j;
			}
		}

	/*
	Run the actual Floyd-Warshall algorithm. Unfortunately it's cubic in the
	number of nodes, so we might need to change it for a different algorithm
	in the future (e.g. running Dijkstra's from each node using a Fib heap).
	This would be an improvement since the graph is generally quite sparse.

	Note that the inductive formula for the next nodes is given by:

	sigma_{ij}^k =	{ sigma_{ij}^{k-1}		if d_{ij}^{k-1} <= d_{ik}^{k-1} + d_{kj}^{k-1}
					{ sigma_{ik}^{k-1}		otherwise
	*/
	for(int k=0; k<size; ++k)
	{
		PathTable_Ptr oldPathTable = pathTable;
		pathTable.reset(new PathTable(size));
		for(int i=0; i<size; ++i)
			for(int j=0; j<size; ++j)
			{
				pathTable->cost(i,j) = std::min(oldPathTable->cost(i,j), oldPathTable->cost(i,k) + oldPathTable->cost(k,j));
				if(oldPathTable->cost(i,j) <= oldPathTable->cost(i,k) + oldPathTable->cost(k,j))
					pathTable->next_node(i,j) = oldPathTable->next_node(i,j);
				else
					pathTable->next_node(i,j) = oldPathTable->next_node(i,k);
			}
	}

	return pathTable;
}

}
