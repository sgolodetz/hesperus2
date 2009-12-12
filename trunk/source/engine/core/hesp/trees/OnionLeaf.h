/***
 * hesperus: OnionLeaf.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ONIONLEAF
#define H_HESP_ONIONLEAF

#include <set>

#include <boost/dynamic_bitset.hpp>

#include "OnionNode.h"

namespace hesp {

class OnionLeaf : public OnionNode
{
	//#################### FRIENDS ####################
	friend class OnionTree;

	//#################### PRIVATE VARIABLES ####################
private:
	int m_leafIndex;								// leaf index of leaf (as opposed to its tree node index): only empty leaves are indexed
	boost::dynamic_bitset<> m_solidityDescriptor;	// indicates the solidity of the leaf in each of the various different maps
	std::vector<int> m_polygonIndices;

	//#################### CONSTRUCTORS ####################
public:
	OnionLeaf(int index, const boost::dynamic_bitset<>& solidityDescriptor, const std::vector<int>& polygonIndices);

	//#################### PUBLIC METHODS ####################
public:
	OnionBranch *as_branch();
	const OnionBranch *as_branch() const;
	OnionLeaf *as_leaf();
	const OnionLeaf *as_leaf() const;
	bool is_completely_solid() const;
	bool is_leaf() const;
	bool is_solid(int mapIndex) const;
	int leaf_index() const;
	std::set<int> map_indices() const;
	void output_postorder_text(std::ostream& os) const;
	const std::vector<int>& polygon_indices() const;
	const boost::dynamic_bitset<>& solidity_descriptor() const;

	//#################### PRIVATE METHODS ####################
private:
	void set_leaf_index(int leafIndex);
};

}

#endif
