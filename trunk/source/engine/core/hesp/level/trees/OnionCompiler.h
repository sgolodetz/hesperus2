/***
 * hesperus: OnionCompiler.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ONIONCOMPILER
#define H_HESP_ONIONCOMPILER

#include <vector>

#include <hesp/math/geom/Plane.h>
#include "OnionTree.h"

namespace hesp {

template <typename Poly>
class OnionCompiler
{
	//#################### TYPEDEFS ####################
private:
	typedef shared_ptr<Poly> Poly_Ptr;
	typedef std::vector<Poly_Ptr> PolyVector;
	typedef shared_ptr<PolyVector> PolyVector_Ptr;

	//#################### NESTED CLASSES ####################
private:
	struct PolyIndex
	{
		int index;
		bool splitCandidate;	// is the plane of the referenced polygon a split candidate?

		PolyIndex(int index_, bool splitCandidate_)
		:	index(index_), splitCandidate(splitCandidate_)
		{}
	};

	typedef shared_ptr<const PolyIndex> PolyIndex_CPtr;

	struct NullAuxData {};

	//#################### PRIVATE VARIABLES ####################
private:
	// Input data
	int m_mapCount;
	std::vector<BSPTree_CPtr> m_mapTrees;
	double m_weight;

	// Intermediate data
	PolyVector_Ptr m_polygons;
	std::vector<PolyIndex> m_polyIndices;

	// Output data
	OnionTree_Ptr m_tree;

	//#################### CONSTRUCTORS ####################
public:
	OnionCompiler(const std::vector<PolyVector>& maps, const std::vector<BSPTree_CPtr>& mapTrees, double weight);

	//#################### PUBLIC METHODS ####################
public:
	void build_tree();
	const PolyVector_Ptr& polygons() const;
	const OnionTree_Ptr& tree() const;

	//#################### PRIVATE METHODS ####################
private:
	OnionNode_Ptr build_subtree(const std::vector<PolyIndex>& polyIndices, std::vector<OnionNode_Ptr>& nodes, std::vector<Plane_Ptr>& ancestorPlanes);
	PolyIndex_CPtr choose_split_poly(const std::vector<PolyIndex>& polyIndices) const;
	boost::dynamic_bitset<> determine_leaf_solidity(const std::vector<Plane_Ptr>& ancestorPlanes) const;
	static Vector3d find_arbitrary_leaf_point(const std::vector<Plane_Ptr>& ancestorPlanes);
};

}

#include "OnionCompiler.tpp"

#endif
