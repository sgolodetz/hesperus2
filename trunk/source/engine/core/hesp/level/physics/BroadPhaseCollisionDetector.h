/***
 * hesperus: BroadPhaseCollisionDetector.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_BROADPHASECOLLISIONDETECTOR
#define H_HESP_BROADPHASECOLLISIONDETECTOR

#include <map>
#include <set>
#include <utility>
#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <hesp/math/vectors/Vector3.h>
#include "PhysicsObject.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<const class BoundsManager> BoundsManager_CPtr;
typedef shared_ptr<const class Contact> Contact_CPtr;
typedef shared_ptr<class PhysicsObject> PhysicsObject_Ptr;

class BroadPhaseCollisionDetector
{
	//#################### TYPEDEFS ####################
private:
	typedef std::pair<PhysicsObject_Ptr,PhysicsObject_Ptr> ObjectPair;

	//#################### NESTED CLASSES ####################
private:
	struct CellIndex
	{
		int i, j, k;

		CellIndex(int i_, int j_, int k_) : i(i_), j(j_), k(k_) {}

		bool operator<(const CellIndex& rhs) const
		{
			return	i < rhs.i ||
					(i == rhs.i && j < rhs.j) ||
					(i == rhs.i && j == rhs.j && k < rhs.k);
		}
	};

	struct ObjectPairPred
	{
		bool operator()(const ObjectPair& lhs, const ObjectPair& rhs) const
		{
			return	lhs.first->id() < rhs.first->id() ||
					(lhs.first->id() == rhs.first->id() && lhs.second->id() < rhs.second->id());
		}
	};

	//#################### TYPEDEFS ####################
private:
	typedef std::vector<std::pair<PhysicsObject_Ptr,bool> > CellData;
	typedef std::map<CellIndex,CellData> Grid;
	typedef std::map<double,Grid> HGrid;
public:
	typedef std::set<ObjectPair,ObjectPairPred> ObjectPairs;

	//#################### PRIVATE VARIABLES ####################
private:
	BoundsManager_CPtr m_boundsManager;
	HGrid m_hgrid;
	ObjectPairs m_potentialCollisions;

	//#################### CONSTRUCTORS ####################
public:
	BroadPhaseCollisionDetector(const BoundsManager_CPtr& boundsManager, double minObjectSize = 0.1, double maxObjectSize = 51.2);

	//#################### PUBLIC METHODS ####################
public:
	void add_object(const PhysicsObject_Ptr& object);
	const ObjectPairs& potential_collisions() const;
	void reset();

	//#################### PRIVATE METHODS ####################
private:
	static CellIndex determine_cell(const Vector3d& p, double gridSize);
	static std::set<CellIndex> determine_overlapped_cells(const Vector3d& mins, const Vector3d& maxs, double gridSize);
	static CellIndex propagate_cell(const CellIndex& cell);
	static std::set<CellIndex> propagate_cells(const std::set<CellIndex>& cells);
};

}

#endif
