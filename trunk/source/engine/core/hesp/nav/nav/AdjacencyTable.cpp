/***
 * hesperus: AdjacencyTable.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "AdjacencyTable.h"

#include <hesp/exceptions/Exception.h>
#include "AdjacencyList.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
AdjacencyTable::AdjacencyTable(const AdjacencyList& adjList)
:	m_size(adjList.size())
{
	m_table.resize(m_size);
	for(int i=0; i<m_size; ++i)
	{
		m_table[i].resize(m_size, (float)INT_MAX);	// use a very large weight to represent +inf (i.e. no edge)
		m_table[i][i] = 0.0f;

		const std::list<AdjacencyList::Edge>& adjEdges = adjList.adjacent_edges(i);
		for(std::list<AdjacencyList::Edge>::const_iterator jt=adjEdges.begin(), jend=adjEdges.end(); jt!=jend; ++jt)
		{
			m_table[i][jt->to_node()] = jt->length();
		}
	}
}

//#################### PUBLIC OPERATORS ####################
/**
Returns the weight of the edge from node i to node j.

@param i			The from node
@param j			The to node
@return				As stated
@throws Exception	If i or j is out of range
*/
float AdjacencyTable::operator()(int i, int j) const
{
	if(i < 0 || i >= m_size || j < 0 || j >= m_size) throw Exception("AdjacencyTable: Index out of range");
	else return m_table[i][j];
}

//#################### PUBLIC METHODS ####################
int AdjacencyTable::size() const
{
	return m_size;
}

}
