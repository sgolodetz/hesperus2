/***
 * hesperus: Skeleton.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_SKELETON
#define H_HESP_SKELETON

#include <map>
#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<const class Animation> Animation_CPtr;
typedef shared_ptr<class BoneHierarchy> BoneHierarchy_Ptr;
typedef shared_ptr<const class BoneHierarchy> BoneHierarchy_CPtr;
typedef shared_ptr<const class ConfiguredPose> ConfiguredPose_CPtr;
typedef shared_ptr<class Pose> Pose_Ptr;
typedef shared_ptr<const class RBTMatrix> RBTMatrix_CPtr;

class Skeleton
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::map<std::string,Animation_CPtr> m_animations;
	BoneHierarchy_Ptr m_boneHierarchy;
	ConfiguredPose_CPtr m_pose;

	// In order to do mesh skinning, we need to be able to move points into the
	// (rest) coordinate frame of each bone. These matrices fulfil that role.
	std::vector<RBTMatrix_CPtr> m_toBoneMatrices;

	//#################### CONSTRUCTORS ####################
public:
	Skeleton(const BoneHierarchy_Ptr& boneHierarchy, const std::map<std::string,Animation_CPtr>& animations);

	//#################### PUBLIC METHODS ####################
public:
	Animation_CPtr animation(const std::string& name) const;
	const BoneHierarchy_Ptr& bone_hierarchy();
	BoneHierarchy_CPtr bone_hierarchy() const;
	const ConfiguredPose_CPtr& get_pose() const;
	bool has_animation(const std::string& name) const;
	Pose_Ptr make_rest_pose() const;
	void render_bones() const;
	void set_pose(const ConfiguredPose_CPtr& pose);
	RBTMatrix_CPtr to_bone_matrix(int i) const;

	//#################### PRIVATE METHODS ####################
private:
	void build_to_bone_matrices();
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Skeleton> Skeleton_Ptr;
typedef shared_ptr<const Skeleton> Skeleton_CPtr;

}

#endif
