/***
 * hesperus: GlobalPathfinder.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "GlobalPathfinder.h"

#include <queue>

#include "NavLink.h"
#include "NavMesh.h"
#include "NavPolygon.h"
#include "PathTable.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
GlobalPathfinder::GlobalPathfinder(const NavMesh_CPtr& navMesh, const AdjacencyList_CPtr& adjList,
								   const PathTable_CPtr& pathTable)
:	m_navMesh(navMesh), m_adjList(adjList), m_pathTable(pathTable)
{}

//#################### PUBLIC METHODS ####################
/**
Tries to find a (high-level) path from sourcePos in sourcePoly to destPos in destPoly.

@param sourcePos	The source position
@param sourcePoly	The source nav polygon
@param destPos		The destination position
@param destPoly		The destination nav polygon
@param path			Used to return the path (if found) to the caller
@return				true, if a path was found, or false otherwise
*/
bool GlobalPathfinder::find_path(const Vector3d& sourcePos, int sourcePoly,
								 const Vector3d& destPos, int destPoly,
								 std::list<int>& path) const
{
	// Step 1:	If the source position and dest position are both in the same nav polygon,
	//			the high-level path is empty (the entity just needs to go in a straight line).
	if(sourcePoly == destPoly)
	{
		return true;
	}

	// Step 2:	Find the shortest unblocked path-table path from a source navlink to a dest navlink.
	//			If such a path is found, and it's no more than (say) 25% longer than the optimal path
	//			we'd have if we ignored blocks, then use it.
	const std::vector<NavLink_Ptr>& links = m_navMesh->links();
	const std::vector<NavPolygon_Ptr>& polygons = m_navMesh->polygons();
	const std::vector<int>& sourceLinkIndices = polygons[sourcePoly]->out_links();
	const std::vector<int>& destLinkIndices = polygons[destPoly]->in_links();

	// Work out the costs of all the possible paths and get them ready to be processed in ascending order of cost.
	std::priority_queue<PathDescriptor, std::vector<PathDescriptor>, std::greater<PathDescriptor> > pq;
	int sourceLinkCount = static_cast<int>(sourceLinkIndices.size());
	int destLinkCount = static_cast<int>(destLinkIndices.size());
	for(int i=0; i<sourceLinkCount; ++i)
	{
		int sourceLinkIndex = sourceLinkIndices[i];
		const NavLink_Ptr& sourceLink = links[sourceLinkIndex];
		float sourceCost = static_cast<float>(sourcePos.distance(sourceLink->source_position()));

		for(int j=0; j<destLinkCount; ++j)
		{
			// The cost of going from sourcePos to destPos via the shortest path-table path
			// between the two navlinks is the sum of the cost of going to the source navlink,
			// the cost of going from the source navlink to the dest navlink, and the cost of
			// going from the dest navlink to destPos.
			int destLinkIndex = destLinkIndices[j];
			const NavLink_Ptr& destLink = links[destLinkIndex];
			float destCost = static_cast<float>(destPos.distance(destLink->dest_position()));
			float interlinkCost = m_pathTable->cost(sourceLinkIndex, destLinkIndex);
			pq.push(PathDescriptor(sourceCost + interlinkCost + destCost, sourceLinkIndex, destLinkIndex));
		}
	}

	// Starting from the least costly path (if any), construct it and see whether it's blocked or not. If not, use it.
	if(!pq.empty())
	{
		// We accept path-table paths which are no more than 25% longer than the shortest
		// path-table path we found without considering blocks: they are "good enough",
		// and are MUCH quicker to calculate than paths we might find using A*.
		const float WORST_ACCEPTABLE_COST = 1.25f * pq.top().cost;

		while(!pq.empty())
		{
			PathDescriptor desc = pq.top();
			if(desc.cost > WORST_ACCEPTABLE_COST) break;
			pq.pop();

			path = m_pathTable->construct_path(desc.sourceLink, desc.destLink);

			if(!is_blocked(sourcePos, path, destPos))
				return true;	// note that the path to be returned has already been stored in the out parameter
		}
	}

	// Step 3:	If a reasonable unblocked path-table path does not exist, do an A* search on
	//			the adjacency list representation of the navigation graph. Use a temporary node
	//			in both the source and destination polygons to represent the actual position of
	//			the player.
	// TODO

	// Step 4:	If an A* path was found, use it. Otherwise, no valid path exists.
	// TODO

	// NYI
	return false;
}

//#################### PRIVATE METHODS ####################
bool GlobalPathfinder::is_blocked(const Vector3d& sourcePos, const std::list<int>& potentialPath, const Vector3d& destPos) const
{
	// NYI
	return false;
}

}
