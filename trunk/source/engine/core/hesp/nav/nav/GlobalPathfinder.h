/***
 * hesperus: GlobalPathfinder.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_GLOBALPATHFINDER
#define H_HESP_GLOBALPATHFINDER

#include <list>

#include <hesp/math/vectors/Vector3.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<const class AdjacencyList> AdjacencyList_CPtr;
typedef shared_ptr<const class NavMesh> NavMesh_CPtr;
typedef shared_ptr<const class PathTable> PathTable_CPtr;

/**
This class provides global (high-level) pathfinding, i.e.
it allows us to find a path from one side of the level to
the other. There should be one of these for each navmesh,
i.e. one for each AABB map.
*/
class GlobalPathfinder
{
	//#################### NESTED CLASSES ####################
private:
	struct PathDescriptor
	{
		float cost;
		int sourceLink;
		int destLink;

		PathDescriptor(float cost_, int sourceLink_, int destLink_)
		:	cost(cost_), sourceLink(sourceLink_), destLink(destLink_)
		{}

		bool operator>(const PathDescriptor& rhs) const
		{
			return cost > rhs.cost;
		}
	};

	//#################### PRIVATE VARIABLES ####################
private:
	NavMesh_CPtr m_navMesh;
	AdjacencyList_CPtr m_adjList;
	PathTable_CPtr m_pathTable;

	//#################### CONSTRUCTORS ####################
public:
	GlobalPathfinder(const NavMesh_CPtr& navMesh, const AdjacencyList_CPtr& adjList, const PathTable_CPtr& pathTable);

	//#################### PUBLIC METHODS ####################
public:
	bool find_path(const Vector3d& sourcePos, int sourcePoly, const Vector3d& destPos, int destPoly, std::list<int>& path) const;

	//#################### PRIVATE METHODS ####################
private:
	bool is_blocked(const Vector3d& sourcePos, const std::list<int>& potentialPath, const Vector3d& destPos) const;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<GlobalPathfinder> GlobalPathfinder_Ptr;
typedef shared_ptr<const GlobalPathfinder> GlobalPathfinder_CPtr;

}

#endif
