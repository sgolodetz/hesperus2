/***
 * hesperus: OnionLeaf.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "OnionLeaf.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
OnionLeaf::OnionLeaf(int index, const boost::dynamic_bitset<>& solidityDescriptor, const std::vector<int>& polygonIndices)
:	OnionNode(index), m_leafIndex(-1), m_solidityDescriptor(solidityDescriptor), m_polygonIndices(polygonIndices)
{}

//#################### PUBLIC METHODS ####################
OnionBranch *OnionLeaf::as_branch()					{ return NULL; }
const OnionBranch *OnionLeaf::as_branch() const		{ return NULL; }
OnionLeaf *OnionLeaf::as_leaf()						{ return this; }
const OnionLeaf *OnionLeaf::as_leaf() const			{ return this; }
bool OnionLeaf::is_leaf() const						{ return true; }
bool OnionLeaf::is_completely_solid() const			{ return (~m_solidityDescriptor).none(); }
bool OnionLeaf::is_solid(int mapIndex) const		{ return m_solidityDescriptor.test(mapIndex); }
int OnionLeaf::leaf_index() const					{ return m_leafIndex; }

std::set<int> OnionLeaf::map_indices() const
{
	std::set<int> ret;
	int mapCount = static_cast<int>(m_solidityDescriptor.size());
	for(int i=0; i<mapCount; ++i)
	{
		if(!m_solidityDescriptor.test(i)) ret.insert(i);
	}
	return ret;
}

void OnionLeaf::output_postorder_text(std::ostream& os) const
{
	os << m_index;

	os << " ( ";
	size_t bitCount = m_solidityDescriptor.size();
	for(size_t i=0; i<bitCount; ++i)
	{
		os << m_solidityDescriptor[i];
	}
	os << " ) ";

	os << m_parent->index();

	size_t polyCount = m_polygonIndices.size();
	os << ' ' << polyCount << " [ ";
	for(size_t i=0; i<polyCount; ++i)
	{
		os << m_polygonIndices[i] << ' ';
	}
	os << "]";

	os << '\n';
}

const std::vector<int>& OnionLeaf::polygon_indices() const				{ return m_polygonIndices; }
const boost::dynamic_bitset<>& OnionLeaf::solidity_descriptor() const	{ return m_solidityDescriptor; }

//#################### PRIVATE METHODS ####################
void OnionLeaf::set_leaf_index(int leafIndex)
{
	m_leafIndex = leafIndex;
}

}
