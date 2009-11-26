/***
 * hesperus: BSPCompiler.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_BSPCOMPILEREX
#define H_HESP_BSPCOMPILEREX

#include <vector>

#include <hesp/math/geom/Plane.h>
#include "BSPTree.h"

namespace hesp {

template <typename Poly>
class BSPCompiler
{
	//#################### ENUMERATIONS ####################
private:
	enum SolidityDescriptor
	{
		SD_UNKNOWN,
		SD_EMPTY,
		SD_SOLID
	};

	//#################### NESTED CLASSES ####################
private:
	struct PolyIndex
	{
		int index;
		bool splitCandidate;	// is the plane of the referenced polygon a split candidate?
		bool hint;				// is the referenced polygon a hint polygon?

		PolyIndex(int index_, bool splitCandidate_, bool hint_)
		:	index(index_), splitCandidate(splitCandidate_), hint(hint_)
		{}
	};

	//#################### TYPEDEFS ####################
private:
	typedef shared_ptr<Poly> Poly_Ptr;
	typedef std::vector<Poly_Ptr> PolyVector;
	typedef shared_ptr<const PolyIndex> PolyIndex_CPtr;

	//#################### PRIVATE VARIABLES ####################
private:
	// Input data
	double m_weight;

	// Intermediate data
	PolyVector m_polygons;
	std::vector<PolyIndex> m_polyIndices;

	// Output data
	BSPTree_Ptr m_tree;

	//#################### CONSTRUCTORS ####################
public:
	BSPCompiler(const PolyVector& polygons, const PolyVector& hintPolygons, double weight);

	//#################### PUBLIC METHODS ####################
public:
	void build_tree();
	const PolyVector& polygons() const;
	BSPTree_Ptr tree() const;

	//#################### PRIVATE METHODS ####################
private:
	BSPNode_Ptr build_subtree(const std::vector<PolyIndex>& polyIndices, std::vector<BSPNode_Ptr>& nodes, SolidityDescriptor solidityDescriptor);
	PolyIndex_CPtr choose_split_poly(const std::vector<PolyIndex>& polyIndices) const;
};

}

#include "BSPCompiler.tpp"

#endif
