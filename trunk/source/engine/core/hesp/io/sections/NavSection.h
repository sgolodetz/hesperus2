/***
 * hesperus: NavSection.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_NAVSECTION
#define H_HESP_NAVSECTION

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class AdjacencyList> AdjacencyList_Ptr;
typedef shared_ptr<const class AdjacencyList> AdjacencyList_CPtr;
typedef shared_ptr<class NavManager> NavManager_Ptr;
typedef shared_ptr<const class NavManager> NavManager_CPtr;
typedef shared_ptr<class NavMesh> NavMesh_Ptr;
typedef shared_ptr<const class NavMesh> NavMesh_CPtr;
typedef shared_ptr<class PathTable> PathTable_Ptr;
typedef shared_ptr<const class PathTable> PathTable_CPtr;

class NavSection
{
	//#################### LOADING METHODS ####################
public:
	static NavManager_Ptr load(std::istream& is);

	//#################### SAVING METHODS ####################
public:
	static void save(std::ostream& os, const NavManager_CPtr& navManager);

	//#################### LOADING SUPPORT METHODS ####################
private:
	static AdjacencyList_Ptr read_adjacency_list(std::istream& is);
	static NavMesh_Ptr read_navmesh(std::istream& is);
	static PathTable_Ptr read_path_table(std::istream& is);

	//#################### SAVING SUPPORT METHODS ####################
private:
	static void write_adjacency_list(std::ostream& os, const AdjacencyList_CPtr& adjList);
	static void write_navmesh(std::ostream& os, const NavMesh_CPtr& mesh);
	static void write_path_table(std::ostream& os, const PathTable_CPtr& pathTable);
};

}

#endif
