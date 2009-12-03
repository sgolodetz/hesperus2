/***
 * hesperus: Model.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MODEL
#define H_HESP_MODEL

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<const class AnimationController> AnimationController_CPtr;
typedef shared_ptr<class ConfiguredPose> ConfiguredPose_Ptr;
typedef shared_ptr<const class ConfiguredPose> ConfiguredPose_CPtr;
typedef shared_ptr<class Mesh> Mesh_Ptr;
typedef shared_ptr<class Skeleton> Skeleton_Ptr;
typedef shared_ptr<const class Skeleton> Skeleton_CPtr;

class Model
{
	//#################### PRIVATE VARIABLES ####################
private:
	Mesh_Ptr m_mesh;
	Skeleton_Ptr m_skeleton;

	//#################### CONSTRUCTORS ####################
public:
	Model(const Mesh_Ptr& mesh, const Skeleton_Ptr& skeleton);

	//#################### PUBLIC METHODS ####################
public:
	ConfiguredPose_Ptr configure_pose(const AnimationController_CPtr& animController) const;
	void render(const ConfiguredPose_CPtr& pose) const;
	const Skeleton_Ptr& skeleton();
	Skeleton_CPtr skeleton() const;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Model> Model_Ptr;
typedef shared_ptr<const Model> Model_CPtr;

}

#endif
