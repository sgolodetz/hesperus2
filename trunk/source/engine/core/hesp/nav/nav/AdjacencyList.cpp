/***
 * hesperus: AdjacencyList.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "AdjacencyList.h"

#include <hesp/exceptions/Exception.h>
#include "NavLink.h"
#include "NavMesh.h"
#include "NavPolygon.h"


namespace hesp {

//#################### CONSTRUCTORS ####################
/**
Constructs an empty adjacency list for a graph with the specified size in nodes.

@param size	The number of nodes in the graph
*/
AdjacencyList::AdjacencyList(int size)
:	m_size(size), m_adjacentEdges(size)
{}

/**
Constructs an adjacency list representation of the graph for a navigation mesh.
The nodes of this graph are the links of the navmesh, and appropriate edges are
added between links in the same polygon.

@param navMesh	The navigation mesh
*/
AdjacencyList::AdjacencyList(const NavMesh_Ptr& navMesh)
{
	const std::vector<NavLink_Ptr> links = navMesh->links();
	const std::vector<NavPolygon_Ptr> polygons = navMesh->polygons();

	m_size = static_cast<int>(links.size());
	m_adjacentEdges.resize(m_size);

	size_t polyCount = polygons.size();
	for(size_t i=0; i<polyCount; ++i)
	{
		const std::vector<int>& inLinks = polygons[i]->in_links();
		const std::vector<int>& outLinks = polygons[i]->out_links();

		// Add edges from each in link to each out link in this polygon.
		int inLinkCount = static_cast<int>(inLinks.size());
		int outLinkCount = static_cast<int>(outLinks.size());
		for(int j=0; j<inLinkCount; ++j)
		{
			const NavLink& inLink = *links[inLinks[j]];
			for(int k=0; k<outLinkCount; ++k)
			{
				const NavLink& outLink = *links[outLinks[k]];
				float length = static_cast<float>(inLink.dest_position().distance(outLink.source_position()));
				if(outLink.dest_poly() != inLink.source_poly())
				{
					m_adjacentEdges[inLinks[j]].push_back(Edge(outLinks[k], length));
				}
			}
		}
	}
}

//#################### PUBLIC METHODS ####################
void AdjacencyList::add_edge(int fromNode, const Edge& edge)
{
	if(fromNode < 0 || fromNode >= m_size) throw Exception("add_edge: From node index out of range");
	m_adjacentEdges[fromNode].push_back(edge);
}

const std::list<AdjacencyList::Edge>& AdjacencyList::adjacent_edges(int node) const
{
	if(node < 0 || node >= m_size) throw Exception("adjacent_edges: Node index out of range");
	return m_adjacentEdges[node];
}

int AdjacencyList::size() const
{
	return m_size;
}

}
