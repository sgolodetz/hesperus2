/***
 * hesperus: VisCalculator.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_VISCALCULATOR
#define H_HESP_VISCALCULATOR

#include <map>
#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <hesp/math/geom/Plane.h>
#include <hesp/portals/Portal.h>
#include "VisTable.h"

namespace hesp {

class VisCalculator
{
	//#################### ENUMERATIONS ####################
private:
	enum PortalVisState
	{
		PV_NO,				// these portals definitely can't see each other
		PV_INITIALMAYBE,	// after the initial calculations, it's still possible these portals can see each other
		PV_FLOODFILLMAYBE,	// after the flood fill, it's still possible these portals can see each other
		PV_YES				// these portals definitely can see each other
	};

	//#################### NESTED CLASSES ####################
private:
	struct PortalTriple
	{
		Portal_Ptr source, inter, target;

		PortalTriple(const Portal_Ptr& source_, const Portal_Ptr& inter_, const Portal_Ptr& target_)
		:	source(source_), inter(inter_), target(target_)
		{}
	};

	//#################### TYPEDEFS ####################
private:
	typedef VisTable<PlaneClassifier> ClassifierTable;
	typedef shared_ptr<ClassifierTable> ClassifierTable_Ptr;
	typedef VisTable<PortalVisState> PortalVisTable;
	typedef shared_ptr<PortalVisTable> PortalVisTable_Ptr;

	//#################### PRIVATE VARIABLES ####################
private:
	// Input data
	int m_emptyLeafCount;
	std::vector<Portal_Ptr> m_portals;

	// Intermediate data
	std::map<int,std::vector<int> > m_portalsFromLeaf;
	ClassifierTable_Ptr m_classifiers;
	PortalVisTable_Ptr m_portalVis;

	// Output data
	LeafVisTable_Ptr m_leafVis;

	//#################### CONSTRUCTORS ####################
public:
	VisCalculator(int emptyLeafCount, const std::vector<Portal_Ptr>& portals);

	//#################### PUBLIC METHODS ####################
public:
	LeafVisTable_Ptr calculate_leaf_vis_table();

	//#################### PRIVATE METHODS ####################
private:
	void build_portals_from_leaf_lookup();
	void calculate_portal_pvs(const Portal_Ptr& originalSource);
	void clean_intermediate();
	void flood_fill();
	void flood_from(int originalSource);
	void full_portal_vis();
	void initial_portal_vis();
	int neighbour_leaf(const Portal_Ptr& portal) const;
	int portal_index(const Portal_Ptr& portal) const;
	void portal_to_leaf_vis();
};

}

#endif
