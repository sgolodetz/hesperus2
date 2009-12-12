/***
 * hesperus: PathTable.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "PathTable.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
PathTable::PathTable(int size)
:	m_table(size), m_size(size)
{
	for(int i=0; i<size; ++i)
	{
		m_table[i].resize(size);
		m_table[i][i].cost = 0.0f;
	}
}

//#################### PUBLIC METHODS ####################
/**
Constructs the path between node i and node j.

@param i	The source node
@param j	The destination node
@return		The path between them
*/
std::list<int> PathTable::construct_path(int i, int j) const
{
	std::list<int> path;
	int cur = i;
	while(cur != j)
	{
		path.push_back(cur);
		cur = m_table[cur][j].nextNode;
	}
	path.push_back(j);
	return path;
}

float& PathTable::cost(int i, int j)					{ return m_table[i][j].cost; }
const float& PathTable::cost(int i, int j) const		{ return m_table[i][j].cost; }
int& PathTable::next_node(int i, int j)					{ return m_table[i][j].nextNode; }
const int& PathTable::next_node(int i, int j) const		{ return m_table[i][j].nextNode; }
int PathTable::size() const								{ return m_size; }

}
