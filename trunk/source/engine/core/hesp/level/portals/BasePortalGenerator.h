/***
 * hesperus: BasePortalGenerator.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_BASEPORTALGENERATOR
#define H_HESP_BASEPORTALGENERATOR

#include <list>
#include <vector>

#include <hesp/math/geom/Plane.h>

namespace hesp {

template <typename PortalT, typename TreeT>
class BasePortalGenerator
{
	//#################### TYPEDEFS ####################
private:
	typedef shared_ptr<PortalT> PortalT_Ptr;
	typedef std::list<PortalT_Ptr> PortalTList;
	typedef shared_ptr<PortalTList> PortalTList_Ptr;
	typedef shared_ptr<const TreeT> TreeT_CPtr;
	typedef typename TreeT::Branch BranchT;
	typedef typename TreeT::Leaf LeafT;
	typedef typename TreeT::Node NodeT;
	typedef shared_ptr<const NodeT> NodeT_CPtr;

	//#################### PRIVATE ABSTRACT METHODS ####################
private:
	virtual PortalTList clip_portal_to_leaf(const PortalT_Ptr& portal, const LeafT *leaf, PlaneClassifier relativeToPortal) const = 0;

	//#################### DESTRUCTOR ####################
public:
	virtual ~BasePortalGenerator() {}

	//#################### PUBLIC METHODS ####################
public:
	PortalTList_Ptr generate_portals(const TreeT_CPtr& tree) const;

	//#################### PRIVATE METHODS ####################
private:
	PortalTList clip_portal_to_subtree(const PortalT_Ptr& portal, const NodeT_CPtr& subtreeRoot, PlaneClassifier relativeToPortal = CP_STRADDLE) const;
	PortalTList clip_portal_to_tree(const PortalT_Ptr& portal, const TreeT_CPtr& tree) const;
	PortalT_Ptr make_initial_portal(const Plane& plane) const;
};

}

#include "BasePortalGenerator.tpp"

#endif
