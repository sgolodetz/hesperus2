/***
 * hesperus: Mesh.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MESH
#define H_HESP_MESH

#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<const class Skeleton> Skeleton_CPtr;
typedef shared_ptr<class Submesh> Submesh_Ptr;

class Mesh
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::vector<Submesh_Ptr> m_submeshes;

	//#################### CONSTRUCTORS ####################
public:
	explicit Mesh(const std::vector<Submesh_Ptr>& submeshes);

	//#################### PUBLIC METHODS ####################
public:
	void render() const;
	void skin(const Skeleton_CPtr& skeleton);
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Mesh> Mesh_Ptr;
typedef shared_ptr<const Mesh> Mesh_CPtr;

}

#endif
