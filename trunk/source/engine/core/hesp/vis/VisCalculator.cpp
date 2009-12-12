/***
 * hesperus: VisCalculator.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "VisCalculator.h"

#include <stack>

#include <hesp/math/geom/GeomUtil.h>
#include "Antipenumbra.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
VisCalculator::VisCalculator(int emptyLeafCount, const std::vector<Portal_Ptr>& portals)
:	m_emptyLeafCount(emptyLeafCount), m_portals(portals)
{
	// Fill in the portal indices: these will be needed later.
	int portalCount = static_cast<int>(m_portals.size());
	for(int i=0; i<portalCount; ++i)
	{
		m_portals[i]->auxiliary_data().index = i;
	}
}

//#################### PUBLIC METHODS ####################
/**
Calculates the full leaf visibility table, which indicates which BSP leaves
can see each other in the game world.

@return	The leaf visibility table
*/
LeafVisTable_Ptr VisCalculator::calculate_leaf_vis_table()
{
	if(!m_leafVis)
	{
		build_portals_from_leaf_lookup();
		initial_portal_vis();
		flood_fill();
		full_portal_vis();
		portal_to_leaf_vis();
		clean_intermediate();
	}

	return m_leafVis;
}

//#################### PRIVATE METHODS ####################
/**
Builds a table which allows us to look up which portals lead
out of a given leaf.
*/
void VisCalculator::build_portals_from_leaf_lookup()
{
	int portalCount = static_cast<int>(m_portals.size());
	for(int i=0; i<portalCount; ++i)
	{
		int fromLeaf = m_portals[i]->auxiliary_data().fromLeaf;
		m_portalsFromLeaf[fromLeaf].push_back(i);
	}
}

/**
Calculates the set of portals that are potentially visible from
the specified portal, and updates the portal visibility table
accordingly.

@param originalSource	The portal for which to calculate the PVS
*/
void VisCalculator::calculate_portal_pvs(const Portal_Ptr& originalSource)
{
	int originalSourceIndex = portal_index(originalSource);
	Plane originalSourcePlane = make_plane(*originalSource);

	std::stack<PortalTriple> st;

	// Initialise the stack with triples targeting all the portals that can be
	// seen from the original source. If only part of a target portal can be
	// seen, we simply split it.
	const std::vector<int>& originalCandidates = m_portalsFromLeaf[neighbour_leaf(originalSource)];
	for(size_t i=0, size=originalCandidates.size(); i<size; ++i)
	{
		Portal_Ptr target = m_portals[originalCandidates[i]];
		int targetIndex = originalCandidates[i];
		if((*m_portalVis)(originalSourceIndex, targetIndex) != PV_NO)
		{
			if((*m_classifiers)(originalSourceIndex, targetIndex) == CP_STRADDLE)
			{
				target = split_polygon(*target, originalSourcePlane).front;
			}
			st.push(PortalTriple(originalSource, Portal_Ptr(), target));
			(*m_portalVis)(originalSourceIndex, targetIndex) = PV_YES;
		}
	}

	// Run the actual visibility calculation process.
	while(!st.empty())
	{
		PortalTriple triple = st.top();
		Portal_Ptr source = triple.source, inter = triple.inter, target = triple.target;
		int targetIndex = portal_index(target);
		st.pop();

		Antipenumbra ap(source, target);

		const std::vector<int>& candidates = m_portalsFromLeaf[neighbour_leaf(target)];
		for(size_t i=0, size=candidates.size(); i<size; ++i)
		{
			Portal_Ptr generator = m_portals[candidates[i]];
			int generatorIndex = candidates[i];

			// If this generator portal might be visible from both the intermediate portal
			// (if it exists) and the target portal, then we need to clip it to find out.
			if((!inter || (*m_portalVis)(portal_index(inter), generatorIndex) != PV_NO) &&
			   ((*m_portalVis)(targetIndex, generatorIndex) != PV_NO))
			{
				Portal_Ptr clippedGen = ap.clip(generator);
				if(clippedGen)
				{
					Antipenumbra reverseAp(clippedGen->flipped_winding(), target);
					Portal_Ptr clippedSrc = reverseAp.clip(source);
					if(clippedSrc)
					{
						st.push(PortalTriple(clippedSrc, target, clippedGen));
						(*m_portalVis)(originalSourceIndex, generatorIndex) = PV_YES;
					}
				}
			}
		}
	}

	// Any portals which haven't been definitely marked as potentially visible at this point can't be seen.
	int portalCount = static_cast<int>(m_portals.size());
	for(int i=0; i<portalCount; ++i)
	{
		if((*m_portalVis)(originalSourceIndex,i) != PV_YES)
			(*m_portalVis)(originalSourceIndex,i) = PV_NO;
	}
}

/**
Clears all the intermediate data structures used during the calculation process.
*/
void VisCalculator::clean_intermediate()
{
	m_portalsFromLeaf.clear();
	m_classifiers.reset();
	m_portalVis.reset();
}

/**
Performs the second phase of the visibility calculation process,
namely flood filling. This is used to refine the initial portal
visibility table before calculating the final version, the aim
being to speed up the final calculation process.
*/
void VisCalculator::flood_fill()
{
	int portalCount = static_cast<int>(m_portals.size());
	for(int i=0; i<portalCount; ++i)
	{
		flood_from(i);

		// If any portals previously thought possible didn't get marked by the flood fill,
		// then they're not actually possible and need to be marked as such.
		for(int j=0; j<portalCount; ++j)
		{
			if((*m_portalVis)(i,j) == PV_INITIALMAYBE)
				(*m_portalVis)(i,j) = PV_NO;
		}
	}
}

/**
Peforms a flood fill from a given portal to refine its approximate
PVS before it is calculated for real.

@param originalSource	The portal from which to flood fill
*/
void VisCalculator::flood_from(int originalSource)
{
	std::stack<int> st;
	st.push(originalSource);

	while(!st.empty())
	{
		int curPortal = st.top();
		st.pop();

		if(curPortal != originalSource)
			(*m_portalVis)(originalSource, curPortal) = PV_FLOODFILLMAYBE;

		int leaf = m_portals[curPortal]->auxiliary_data().toLeaf;
		const std::vector<int>& candidates = m_portalsFromLeaf[leaf];
		for(size_t i=0, size=candidates.size(); i<size; ++i)
		{
			if((*m_portalVis)(originalSource, candidates[i]) == PV_INITIALMAYBE)
				st.push(candidates[i]);
		}
	}
}

/**
Performs the third (final) phase of the visibility calculation process,
in which the real PVS is calculated for each portal.
*/
void VisCalculator::full_portal_vis()
{
	// FIXME:	We should process the portals in ascending order of the
	//			number of other portals they can potentially see: this
	//			will speed the whole thing up.

	int portalCount = static_cast<int>(m_portals.size());
	for(int i=0; i<portalCount; ++i)
	{
		calculate_portal_pvs(m_portals[i]);
	}
}

/**
Performs the first phase of the visibility calculation process. In this
phase, portals which obviously can't see each other (e.g. one portal
is fully behind another) are marked as such in the portal visibility
table. This helps avoid a lot of unnecessary clipping later on.
*/
void VisCalculator::initial_portal_vis()
{
	int portalCount = static_cast<int>(m_portals.size());
	m_portalVis.reset(new PortalVisTable(portalCount, PV_INITIALMAYBE));

	// Calculate the classification relation between the portals. Specifically,
	// classifiers(i,j) will contain the classification of polygon j relative
	// to the plane of i.

	// Note:	This bit could potentially be optimized if we required that portal pairs
	//			occupied consecutive indices in the list (e.g. if 1 were necessarily the
	//			reverse portal of 0, etc.).
	m_classifiers.reset(new ClassifierTable(portalCount));
	for(int i=0; i<portalCount; ++i)
	{
		const Plane plane = make_plane(*m_portals[i]);
		for(int j=0; j<portalCount; ++j)
		{
			if(j == i) (*m_classifiers)(i,j) = CP_COPLANAR;
			else (*m_classifiers)(i,j) = classify_polygon_against_plane(*m_portals[j], plane);
		}
	}

	// Run through the portal visibility table and mark (*m_portalVis)(i,j) as PV_NO
	// if portal i definitely can't see through portal j.
	for(int i=0; i<portalCount; ++i)
	{
		for(int j=0; j<portalCount; ++j)
		{
			if(j == i)
			{
				(*m_portalVis)(i,j) = PV_NO;
				continue;
			}

			// Note: Portals can only see through the back of other portals.

			// If portal j is behind or on the plane of portal i, then i can't see it.
			if((*m_classifiers)(i,j) == CP_BACK || (*m_classifiers)(i,j) == CP_COPLANAR)
				(*m_portalVis)(i,j) = PV_NO;

			// If portal i is completely in front of portal j, then it's facing i
			// and i can't see through it.
			if((*m_classifiers)(j,i) == CP_FRONT)
				(*m_portalVis)(i,j) = PV_NO;
		}
	}
}

/**
Returns the leaf into which the specified portal is facing.

@param portal	The portal
@return			The index of the leaf into which the portal is facing
*/
int VisCalculator::neighbour_leaf(const Portal_Ptr& portal) const
{
	return portal->auxiliary_data().toLeaf;
}

/**
Returns the index of the specified portal.

@param portal	The portal
@return			As stated
*/
int VisCalculator::portal_index(const Portal_Ptr& portal) const
{
	return portal->auxiliary_data().index;
}

/**
Constructs a leaf visibility table from the portal one. The result of
the visibility calculation process is actually this leaf visibility
table, not the portal one, but the latter is more convenient during
the calculation process itself: we thus convert the one to the other
once we've finished calculating.
*/
void VisCalculator::portal_to_leaf_vis()
{
	const int portalCount = static_cast<int>(m_portals.size());

	m_leafVis.reset(new LeafVisTable(m_emptyLeafCount, LEAFVIS_NO));

	for(int i=0; i<m_emptyLeafCount; ++i)
	{
		// Leaf i can see itself, plus the union of whatever leaves its portals can see.
		(*m_leafVis)(i, i) = LEAFVIS_YES;

		const std::vector<int>& ps = m_portalsFromLeaf[i];
		for(std::vector<int>::const_iterator jt=ps.begin(), jend=ps.end(); jt!=jend; ++jt)
		{
			const int j = *jt;

			// Leaf i can see the leaf pointed to by portal j (even though portal j can't see itself).
			(*m_leafVis)(i, m_portals[j]->auxiliary_data().toLeaf) = LEAFVIS_YES;

			// Leaf i can see all the leaves pointed to by portals portal j can see.
			for(int k=0; k<portalCount; ++k)
			{
				if((*m_portalVis)(j,k) == PV_YES)
				{
					(*m_leafVis)(i, m_portals[k]->auxiliary_data().toLeaf) = LEAFVIS_YES;
				}
			}
		}
	}
}

}
