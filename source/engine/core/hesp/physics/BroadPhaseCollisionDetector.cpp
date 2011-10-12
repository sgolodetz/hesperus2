/***
 * hesperus: BroadPhaseCollisionDetector.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "BroadPhaseCollisionDetector.h"

#include <cmath>
#include <limits>

#include <hesp/bounds/Bounds.h>
#include "PhysicsObject.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
BroadPhaseCollisionDetector::BroadPhaseCollisionDetector(const BoundsManager_CPtr& boundsManager,
														 double minObjectSize, double maxObjectSize)
:	m_boundsManager(boundsManager)
{
	for(double gridSize=minObjectSize; gridSize<maxObjectSize*2; gridSize*=2)
	{
		m_hgrid.insert(std::make_pair(gridSize, Grid()));
	}

	m_hgrid.insert(std::make_pair(std::numeric_limits<double>::max(), Grid()));
}

//#################### PUBLIC METHODS ####################
void BroadPhaseCollisionDetector::add_object(const PhysicsObject_Ptr& object)
{
	// Step 1:	Determine the lowest grid for the object, based on the maximum dimension of
	// the bounding box of its movement.
	Vector3d halfDimensions = object->bounds(m_boundsManager)->half_dimensions();
	Vector3d previousPos = object->previous_position() ? *object->previous_position() : object->position();
	Vector3d pos = object->position();

	Vector3d mins(std::min(previousPos.x, pos.x), std::min(previousPos.y, pos.y), std::min(previousPos.z, pos.z));
	mins -= halfDimensions;
	Vector3d maxs(std::max(previousPos.x, pos.x), std::max(previousPos.y, pos.y), std::max(previousPos.z, pos.z));
	maxs += halfDimensions;

	Vector3d size = maxs - mins;
	double maxDimension = std::max(std::max(size.x, size.y), size.z);

	HGrid::iterator lowestGrid = m_hgrid.lower_bound(maxDimension);

	// Step 2:	Determine which cells it overlaps on the lowest grid just identified.
	std::set<CellIndex> cells = determine_overlapped_cells(mins, maxs, lowestGrid->first);

	// Step 3:	Insert the object into every grid above (and including) the lowest one,
	//			flagging collisions where applicable.
	for(HGrid::iterator it=lowestGrid, iend=m_hgrid.end(); it!=iend; ++it)
	{
		bool lowest = it == lowestGrid;

		if(!lowest)
		{
			// Propagate the cells up to the next layer.
			cells = propagate_cells(cells);
		}

		for(std::set<CellIndex>::const_iterator jt=cells.begin(), jend=cells.end(); jt!=jend; ++jt)
		{
			CellData& data = it->second[*jt];

			// Look through the objects already overlapping the cell and flag any necessary collisions.
			for(CellData::const_iterator kt=data.begin(), kend=data.end(); kt!=kend; ++kt)
			{
				if(lowest || kt->second)	// if this is the lowest layer for at least one of the objects
				{
					m_potentialCollisions.insert(std::make_pair(kt->first, object));
				}
			}

			// Add the object itself to the set of those overlapping the cell.
			data.push_back(std::make_pair(object, lowest));
		}
	}
}

const BroadPhaseCollisionDetector::ObjectPairs& BroadPhaseCollisionDetector::potential_collisions() const
{
	return m_potentialCollisions;
}

void BroadPhaseCollisionDetector::reset()
{
	for(HGrid::iterator it=m_hgrid.begin(), iend=m_hgrid.end(); it!=iend; ++it)
	{
		it->second.clear();
	}

	m_potentialCollisions.clear();
}

//#################### PRIVATE METHODS ####################
BroadPhaseCollisionDetector::CellIndex
BroadPhaseCollisionDetector::determine_cell(const Vector3d& p, double gridSize)
{
	return CellIndex(static_cast<int>(floor(p.x / gridSize)),
					 static_cast<int>(floor(p.y / gridSize)),
					 static_cast<int>(floor(p.z / gridSize)));
}

std::set<BroadPhaseCollisionDetector::CellIndex>
BroadPhaseCollisionDetector::determine_overlapped_cells(const Vector3d& mins, const Vector3d& maxs, double gridSize)
{
	CellIndex minCell = determine_cell(mins, gridSize);
	CellIndex maxCell = determine_cell(maxs, gridSize);

	std::set<CellIndex> ret;
	for(int i=minCell.i; i<=maxCell.i; ++i)
		for(int j=minCell.j; j<=maxCell.j; ++j)
			for(int k=minCell.k; k<=maxCell.k; ++k)
			{
				ret.insert(CellIndex(i,j,k));
			}
	return ret;
}

BroadPhaseCollisionDetector::CellIndex
BroadPhaseCollisionDetector::propagate_cell(const CellIndex& cell)
{
	return CellIndex(static_cast<int>(floor(cell.i*0.5)),
					 static_cast<int>(floor(cell.j*0.5)),
					 static_cast<int>(floor(cell.k*0.5)));
}

std::set<BroadPhaseCollisionDetector::CellIndex>
BroadPhaseCollisionDetector::propagate_cells(const std::set<CellIndex>& cells)
{
	std::set<CellIndex> ret;
	for(std::set<CellIndex>::const_iterator it=cells.begin(), iend=cells.end(); it!=iend; ++it)
	{
		ret.insert(propagate_cell(*it));
	}
	return ret;
}

}
