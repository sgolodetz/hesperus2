/***
 * hesperus: OnionNode.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ONIONNODE
#define H_HESP_ONIONNODE

#include <iosfwd>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
class OnionBranch;
class OnionLeaf;

class OnionNode
{
	//#################### PROTECTED VARIABLES ####################
protected:
	int m_index;
	OnionNode *m_parent;

	//#################### CONSTRUCTORS ####################
public:
	OnionNode(int index);

	//#################### DESTRUCTOR ####################
public:
	virtual ~OnionNode() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual OnionBranch *as_branch() = 0;
	virtual const OnionBranch *as_branch() const = 0;
	virtual OnionLeaf *as_leaf() = 0;
	virtual const OnionLeaf *as_leaf() const = 0;
	virtual bool is_leaf() const = 0;
	virtual void output_postorder_text(std::ostream& os) const = 0;

	//#################### PUBLIC METHODS ####################
public:
	int index() const;
	const OnionNode *parent() const;
	void set_parent(OnionNode *parent);
};

//#################### TYPEDEFS ####################
typedef shared_ptr<OnionNode> OnionNode_Ptr;
typedef shared_ptr<const OnionNode> OnionNode_CPtr;

}

#endif
