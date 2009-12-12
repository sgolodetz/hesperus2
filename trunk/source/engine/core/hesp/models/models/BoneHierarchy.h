/***
 * hesperus: BoneHierarchy.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_BONEHIERARCHY
#define H_HESP_BONEHIERARCHY

#include <map>
#include <vector>

#include "PoseModifier.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class Bone> Bone_Ptr;
typedef shared_ptr<const class Bone> Bone_CPtr;
typedef shared_ptr<class ConfiguredPose> ConfiguredPose_Ptr;
typedef shared_ptr<const class Pose> Pose_CPtr;
typedef shared_ptr<const class RBTMatrix> RBTMatrix_CPtr;

//#################### TYPEDEFS ####################
typedef shared_ptr<class BoneHierarchy> BoneHierarchy_Ptr;
typedef shared_ptr<const class BoneHierarchy> BoneHierarchy_CPtr;

class BoneHierarchy
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::vector<Bone_Ptr> m_bones;
	std::map<std::string,int> m_boneLookup;

	//#################### CONSTRUCTORS ####################
public:
	explicit BoneHierarchy(const std::vector<Bone_Ptr>& bones);

	//#################### PUBLIC METHODS ####################
public:
	void attach_to_parent(const BoneHierarchy_Ptr& parent, const std::string& parentBoneName);
	int bone_count() const;
	Bone_Ptr bones(int i);
	Bone_CPtr bones(int i) const;
	Bone_Ptr bones(const std::string& name);
	Bone_CPtr bones(const std::string& name) const;
	ConfiguredPose_Ptr configure_pose(const Pose_CPtr& unconfiguredPose, const std::map<std::string,PoseModifier>& modifiers = (std::map<std::string,PoseModifier>())) const;
	void detach_from_parent();
	int find_bone(const std::string& name) const;

	//#################### PRIVATE METHODS ####################
private:
	void calculate_absolute_matrices(const std::map<std::string,PoseModifier>& modifiers) const;
	void calculate_absolute_matrix(const Bone_Ptr& bone, const std::map<std::string,PoseModifier>& modifiers) const;
	void calculate_relative_matrices(const std::vector<RBTMatrix_CPtr>& unconfiguredRelMats) const;
};

}

#endif
