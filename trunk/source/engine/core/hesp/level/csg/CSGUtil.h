/***
 * hesperus: CSGUtil.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CSGUTIL
#define H_HESP_CSGUTIL

#include <list>
#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <hesp/level/brushes/PolyhedralBrush.h>
#include <hesp/level/trees/BSPTree.h>
#include <hesp/math/geom/Polygon.h>

namespace hesp {

template <typename Vert, typename AuxData>
class CSGUtil
{
	//#################### TYPEDEFS ####################
private:
	typedef Polygon<Vert,AuxData> Poly;
	typedef shared_ptr<Poly> Poly_Ptr;
	typedef std::list<Poly_Ptr> PolyList;
	typedef shared_ptr<PolyList> PolyList_Ptr;
	typedef std::vector<Poly_Ptr> PolyVector;

	typedef PolyhedralBrush<Poly> PolyBrush;
	typedef shared_ptr<PolyBrush> PolyBrush_Ptr;
	typedef std::vector<PolyBrush_Ptr> PolyBrushVector;

	//#################### PUBLIC METHODS ####################
public:
	static PolyList clip_polygons_to_tree(const PolyList& polys, const BSPTree_CPtr& tree, bool coplanarFlag);
	static PolyList_Ptr union_all(const PolyBrushVector& brushes);

	//#################### PRIVATE METHODS ####################
private:
	static BSPTree_Ptr build_tree(const PolyBrush& brush);
	static std::pair<PolyList,bool> clip_polygon_to_subtree(const Poly_Ptr& poly, const BSPNode_CPtr& node, bool coplanarFlag);	
};

}

#include "CSGUtil.tpp"

#endif
