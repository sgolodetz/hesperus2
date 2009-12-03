/***
 * hesperus: NavDataset.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_NAVDATASET
#define H_HESP_NAVDATASET

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class AdjacencyList> AdjacencyList_Ptr;
typedef shared_ptr<const class AdjacencyList> AdjacencyList_CPtr;
typedef shared_ptr<class NavMesh> NavMesh_Ptr;
typedef shared_ptr<const class NavMesh> NavMesh_CPtr;
typedef shared_ptr<class PathTable> PathTable_Ptr;
typedef shared_ptr<const class PathTable> PathTable_CPtr;

/**
An instance of this class stores all the necessary nav data for a particular AABB map.
*/
class NavDataset
{
	//#################### PRIVATE VARIABLES ####################
private:
	AdjacencyList_Ptr m_adjList;
	NavMesh_Ptr m_navMesh;
	PathTable_Ptr m_pathTable;

	//#################### CONSTRUCTORS ####################
public:
	NavDataset(const AdjacencyList_Ptr& adjList, const NavMesh_Ptr& navMesh, const PathTable_Ptr& pathTable)
	:	m_adjList(adjList), m_navMesh(navMesh), m_pathTable(pathTable)
	{}

	//#################### PUBLIC METHODS ####################
public:
	const AdjacencyList_Ptr& adjacency_list()	{ return m_adjList; }
	AdjacencyList_CPtr adjacency_list() const	{ return m_adjList; }
	const NavMesh_Ptr& nav_mesh()				{ return m_navMesh; }
	NavMesh_CPtr nav_mesh() const				{ return m_navMesh; }
	const PathTable_Ptr& path_table()			{ return m_pathTable; }
	PathTable_CPtr path_table() const			{ return m_pathTable; }
};

//#################### TYPEDEFS ####################
typedef shared_ptr<NavDataset> NavDataset_Ptr;
typedef shared_ptr<const NavDataset> NavDataset_CPtr;

}

#endif
