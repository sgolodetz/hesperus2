/***
 * hesperus: BSPNode.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_BSPNODE
#define H_HESP_BSPNODE

#include <iosfwd>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
class BSPBranch;
class BSPLeaf;

class BSPNode
{
	//#################### PROTECTED VARIABLES ####################
protected:
	int m_index;
	BSPNode *m_parent;

	//#################### CONSTRUCTORS ####################
public:
	BSPNode(int index);

	//#################### DESTRUCTOR ####################
public:
	virtual ~BSPNode() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual BSPBranch *as_branch() = 0;
	virtual const BSPBranch *as_branch() const = 0;
	virtual BSPLeaf *as_leaf() = 0;
	virtual const BSPLeaf *as_leaf() const = 0;
	virtual bool is_leaf() const = 0;
	virtual void output_postorder_text(std::ostream& os) const = 0;

	//#################### PUBLIC METHODS ####################
public:
	int index() const;
	const BSPNode *parent() const;
	void set_parent(BSPNode *parent);
};

//#################### TYPEDEFS ####################
typedef shared_ptr<BSPNode> BSPNode_Ptr;
typedef shared_ptr<const BSPNode> BSPNode_CPtr;

}

#endif
