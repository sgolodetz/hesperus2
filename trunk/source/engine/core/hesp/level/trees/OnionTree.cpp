/***
 * hesperus: OnionTree.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "OnionTree.h"

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <hesp/exceptions/Exception.h>
#include <hesp/io/util/LineIO.h>
#include <hesp/math/geom/GeomUtil.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
OnionTree::OnionTree(const std::vector<OnionNode_Ptr>& nodes, int mapCount)
:	m_nodes(nodes), m_mapCount(mapCount)
{
	index_leaves();
}

//#################### PUBLIC METHODS ####################
const OnionLeaf *OnionTree::leaf(int n) const
{
	return m_leaves[n];
}

OnionTree_Ptr OnionTree::load_postorder_text(std::istream& is)
{
	std::string line;

	LineIO::portable_getline(is, line);
	int mapCount;
	try							{ mapCount = lexical_cast<int>(line); }
	catch(bad_lexical_cast&)	{ throw Exception("The map count is not a number"); }

	LineIO::portable_getline(is, line);
	int nodeCount;
	try							{ nodeCount = lexical_cast<int>(line); }
	catch(bad_lexical_cast&)	{ throw Exception("The onion node count is not a number"); }

	std::vector<OnionNode_Ptr> nodes(nodeCount);

	int n = 0;
	while(n < nodeCount && LineIO::portable_getline(is, line))
	{
		typedef boost::char_separator<char> sep;
		typedef boost::tokenizer<sep> tokenizer;

		tokenizer tok(line, sep(" "));
		std::vector<std::string> tokens(tok.begin(), tok.end());
		size_t tokenCount = tokens.size();
		if(tokenCount < 2) throw Exception("Bad onion node: " + lexical_cast<std::string>(n));

		if(tokens[1] == "B")
		{
			if(tokenCount != 11 || tokens[5] != "(" || tokens[10] != ")")
				throw Exception("Bad branch node: " + lexical_cast<std::string>(n));

			int leftIndex, rightIndex;
			double a, b, c, d;
			try
			{
				leftIndex = lexical_cast<int>(tokens[2]);
				rightIndex = lexical_cast<int>(tokens[3]);
				a = lexical_cast<double>(tokens[6]);
				b = lexical_cast<double>(tokens[7]);
				c = lexical_cast<double>(tokens[8]);
				d = lexical_cast<double>(tokens[9]);
			}
			catch(bad_lexical_cast&)	{ throw Exception("One of the values was not a number in branch node: " + lexical_cast<std::string>(n)); }

			OnionNode_Ptr left = nodes[leftIndex];
			OnionNode_Ptr right = nodes[rightIndex];
			if(left && right)
			{
				Plane_Ptr splitter(new Plane(Vector3d(a,b,c), d));
				nodes[n] = OnionNode_Ptr(new OnionBranch(n, splitter, left, right));
			}
			else throw Exception("The onion nodes are not stored in postorder: the child nodes for this branch have not yet been loaded");
		}
		else
		{
			// We're dealing with an onion leaf.
			if(tokenCount < 8 || tokens[1] != "(" || tokens[3] != ")" || tokens[6] != "[" || tokens[tokenCount-1] != "]")
				throw Exception("Bad leaf node: " + lexical_cast<std::string>(n));

			if(tokens[2].length() != mapCount)
				throw Exception("Bad leaf solidity descriptor: " + lexical_cast<std::string>(n));

			// Construct the solidity descriptor.
			boost::dynamic_bitset<> solidityDescriptor(mapCount);
			for(size_t i=0, len=tokens[2].length(); i<len; ++i)
			{
				if(tokens[2][i] == '0') solidityDescriptor.set(i, false);
				else if(tokens[2][i] == '1') solidityDescriptor.set(i, true);
				else throw Exception("Bad leaf solidity descriptor: " + lexical_cast<std::string>(n));
			}

			// Read in any polygon indices.
			int polyCount;
			try							{ polyCount = lexical_cast<int>(tokens[5]); }
			catch(bad_lexical_cast&)	{ throw Exception("The leaf polygon count is not a number: " + lexical_cast<std::string>(n)); }

			std::vector<int> polyIndices;

			for(size_t i=7; i<tokenCount-1; ++i)
			{
				int polyIndex;
				try							{ polyIndex = lexical_cast<int>(tokens[i]); }
				catch(bad_lexical_cast&)	{ throw Exception("A polygon index is not a number in leaf: " + lexical_cast<std::string>(n)); }

				polyIndices.push_back(polyIndex);
			}

			nodes[n].reset(new OnionLeaf(n, solidityDescriptor, polyIndices));
		}

		++n;
	}

	return OnionTree_Ptr(new OnionTree(nodes, mapCount));
}

int OnionTree::map_count() const
{
	return m_mapCount;
}

void OnionTree::output_postorder_text(std::ostream& os) const
{
	os << m_mapCount << '\n';
	os << m_nodes.size() << '\n';
	root()->output_postorder_text(os);
}

OnionNode_Ptr OnionTree::root()			{ return m_nodes.back(); }
OnionNode_CPtr OnionTree::root() const	{ return m_nodes.back(); }

//#################### PRIVATE METHODS ####################
void OnionTree::index_leaves()
{
	index_leaves_sub(root());
}

void OnionTree::index_leaves_sub(const OnionNode_Ptr& node)
{
	if(node->is_leaf())
	{
		OnionLeaf *leaf = node->as_leaf();
		int nextLeaf = static_cast<int>(m_leaves.size());
		leaf->set_leaf_index(nextLeaf);
		m_leaves.push_back(leaf);
	}
	else
	{
		OnionBranch *branch = node->as_branch();
		index_leaves_sub(branch->left());
		index_leaves_sub(branch->right());
	}
}

}
