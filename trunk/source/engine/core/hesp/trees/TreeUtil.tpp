/***
 * hesperus: TreeUtil.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <hesp/math/geom/GeomUtil.h>

namespace hesp {

//#################### PUBLIC METHODS ####################
/**
This is needed to allow a pointer to a non-const tree to be passed in to
the function below: without it, the compiler generates an error.
*/
template <typename Tree>
int TreeUtil::find_leaf_index(const Vector3d& p, shared_ptr<Tree> tree)
{
	return find_leaf_index<const Tree>(p, tree);
}

/**
Finds the index of the leaf in the specified tree in which the specified point resides.

@param p		The point
@param tree		The tree
@return			The leaf index
*/
template <typename Tree>
int TreeUtil::find_leaf_index(const Vector3d& p, shared_ptr<const Tree> tree)
{
	typedef typename Tree::Branch Branch;
	typedef typename Tree::Leaf Leaf;
	typedef typename Tree::Node Node;

	shared_ptr<const Node> cur = tree->root();
	while(!cur->is_leaf())
	{
		const Branch *branch = cur->as_branch();
		switch(classify_point_against_plane(p, *branch->splitter()))
		{
			case CP_BACK:
			{
				cur = branch->right();
				break;
			}
			default:	// CP_COPLANAR or CP_FRONT
			{
				cur = branch->left();
				break;
			}
		}
	}

	const Leaf *leaf = cur->as_leaf();
	return leaf->leaf_index();
}

/**
This is needed to allow a pointer to a non-const tree to be passed in to
the function below: without it, the compiler generates an error.
*/
template <typename Tree>
std::list<Plane_CPtr> TreeUtil::split_planes(shared_ptr<Tree> tree)
{
	return split_planes<const Tree>(tree);
}

template <typename Tree>
std::list<Plane_CPtr> TreeUtil::split_planes(shared_ptr<const Tree> tree)
{
	return split_planes_sub<Tree>(tree->root());
}

//#################### PRIVATE METHODS ####################
template <typename Tree>
std::list<Plane_CPtr> TreeUtil::split_planes_sub(shared_ptr<const typename Tree::Node> node)
{
	std::list<Plane_CPtr> ret;

	if(!node->is_leaf())
	{
		const typename Tree::Branch *branch = node->as_branch();
		ret.push_back(branch->splitter());

		std::list<Plane_CPtr> result = split_planes_sub<Tree>(branch->left());
		ret.splice(ret.end(), result);

		result = split_planes_sub<Tree>(branch->right());
		ret.splice(ret.end(), result);
	}

	return ret;
}

}
