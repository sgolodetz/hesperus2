/***
 * hesperus: BSPTree.cpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include "BSPTree.h"

#include <iostream>
#include <string>

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <hesp/exceptions/Exception.h>
#include <hesp/math/geom/GeomUtil.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
BSPTree::BSPTree(const std::vector<BSPNode_Ptr>& nodes)
:	m_nodes(nodes)
{
	index_leaves();
}

//#################### PUBLIC METHODS ####################
/**
Returns the number of empty leaves in the tree.

@return	As stated
*/
int BSPTree::empty_leaf_count() const
{
	return m_emptyLeafCount;
}

/**
Returns the leaf with leaf index n.

@param n			The leaf index
@return				As stated
@throws Exception	If the leaf index is out of range
*/
BSPLeaf *BSPTree::leaf(int n)
{
	int leafCount = static_cast<int>(m_leaves.size());
	if(n >= 0 && n < leafCount) return m_leaves[n];
	else throw Exception("Leaf index out of range");
}

/**
Returns the leaf with leaf index n (as above).
*/
const BSPLeaf *BSPTree::leaf(int n) const
{
	int leafCount = static_cast<int>(m_leaves.size());
	if(n >= 0 && n < leafCount) return m_leaves[n];
	else throw Exception("Leaf index out of range");
}

BSPTree_Ptr BSPTree::load_postorder_text(std::istream& is)
{
	std::string line;
	std::getline(is, line);
	int nodeCount;
	try							{ nodeCount = lexical_cast<int,std::string>(line); }
	catch(bad_lexical_cast&)	{ throw Exception("The BSP node count is not a number"); }

	std::vector<BSPNode_Ptr> nodes(nodeCount);

	int n = 0;
	while(n < nodeCount && std::getline(is, line))
	{
		typedef boost::char_separator<char> sep;
		typedef boost::tokenizer<sep> tokenizer;

		tokenizer tok(line, sep(" "));
		std::vector<std::string> tokens(tok.begin(), tok.end());
		size_t tokenCount = tokens.size();
		if(tokenCount < 2) throw Exception("Bad BSP node: " + lexical_cast<std::string,int>(n));

		if(tokens[1] == "E")
		{
			if(tokenCount < 6 || tokens[4] != "[" || tokens[tokenCount-1] != "]")
				throw Exception("Bad empty leaf node: " + lexical_cast<std::string,int>(n));

			int polyCount;
			try							{ polyCount = lexical_cast<int,std::string>(tokens[3]); }
			catch(bad_lexical_cast&)	{ throw Exception("The leaf polygon count is not a number: " + lexical_cast<std::string,int>(n)); }

			std::vector<int> polyIndices;

			for(size_t i=5; i<tokenCount-1; ++i)
			{
				int polyIndex;
				try							{ polyIndex = lexical_cast<int,std::string>(tokens[i]); }
				catch(bad_lexical_cast&)	{ throw Exception("A polygon index is not a number in leaf: " + lexical_cast<std::string,int>(n)); }

				polyIndices.push_back(polyIndex);
			}

			nodes[n] = BSPLeaf::make_empty_leaf(n, polyIndices);
		}
		else if(tokens[1] == "S")
		{
			nodes[n] = BSPLeaf::make_solid_leaf(n);
		}
		else if(tokens[1] == "B")
		{
			if(tokenCount != 11 || tokens[5] != "(" || tokens[10] != ")")
				throw Exception("Bad branch node: " + lexical_cast<std::string,int>(n));

			int leftIndex, rightIndex;
			double a, b, c, d;
			try
			{
				leftIndex = lexical_cast<int,std::string>(tokens[2]);
				rightIndex = lexical_cast<int,std::string>(tokens[3]);
				a = lexical_cast<double,std::string>(tokens[6]);
				b = lexical_cast<double,std::string>(tokens[7]);
				c = lexical_cast<double,std::string>(tokens[8]);
				d = lexical_cast<double,std::string>(tokens[9]);
			}
			catch(bad_lexical_cast&)	{ throw Exception("One of the values was not a number in branch node: " + lexical_cast<std::string,int>(n)); }

			BSPNode_Ptr left = nodes[leftIndex];
			BSPNode_Ptr right = nodes[rightIndex];
			if(left && right)
			{
				Plane_Ptr splitter(new Plane(Vector3d(a,b,c), d));
				nodes[n] = BSPNode_Ptr(new BSPBranch(n, splitter, left, right));
			}
			else throw Exception("The BSP nodes are not stored in postorder: the child nodes for this branch have not yet been loaded");
		}

		++n;
	}

	return BSPTree_Ptr(new BSPTree(nodes));
}

void BSPTree::output_postorder_text(std::ostream& os) const
{
	os << m_nodes.size() << '\n';
	root()->output_postorder_text(os);
}

BSPNode_Ptr BSPTree::root()			{ return m_nodes.back(); }
BSPNode_CPtr BSPTree::root() const	{ return m_nodes.back(); }

//#################### PRIVATE METHODS ####################
void BSPTree::index_leaves()
{
	index_specific_leaves(root(), false);
	m_emptyLeafCount = static_cast<int>(m_leaves.size());
	index_specific_leaves(root(), true);
}

void BSPTree::index_specific_leaves(const BSPNode_Ptr& node, bool solidFlag)
{
	if(node->is_leaf())
	{
		BSPLeaf *leaf = node->as_leaf();
		if(leaf->is_solid() == solidFlag)
		{
			int nextLeaf = static_cast<int>(m_leaves.size());
			leaf->set_leaf_index(nextLeaf);
			m_leaves.push_back(leaf);
		}
	}
	else
	{
		BSPBranch *branch = node->as_branch();
		index_specific_leaves(branch->left(), solidFlag);
		index_specific_leaves(branch->right(), solidFlag);
	}
}

}
