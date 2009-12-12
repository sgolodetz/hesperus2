/***
 * hesperus: AnimationController.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ANIMATIONCONTROLLER
#define H_HESP_ANIMATIONCONTROLLER

#include <map>

#include "PoseModifier.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<const class Pose> Pose_CPtr;
typedef shared_ptr<const class Skeleton> Skeleton_CPtr;

class AnimationController
{
	//#################### ENUMERATIONS ####################
private:
	enum State
	{
		AS_REST,					// not playing any animation
		AS_PLAY,					// playing an animation
		AS_TRANSITION				// transitioning between animations
	};

	//#################### PRIVATE VARIABLES ####################
private:
	Skeleton_CPtr m_skeleton;
	bool m_interpolateKeyframes;

	State m_state;
	std::string m_animationName;	// the name of the current animation
	int m_animationTime;			// the number of ms for which the current animation (or transition) has been playing
	Pose_CPtr m_pose;				// the current pose
	Pose_CPtr m_transitionStart;	// the pose at the start of the transition

	std::map<std::string,PoseModifier> m_poseModifiers;

	//#################### CONSTRUCTORS ####################
public:
	AnimationController(bool interpolateKeyframes = false);

	//#################### PUBLIC METHODS ####################
public:
	void clear_pose_modifiers();
	const Pose_CPtr& get_pose() const;
	const std::map<std::string,PoseModifier>& get_pose_modifiers() const;
	void remove_pose_modifier(const std::string& boneName);
	void request_animation(std::string newAnimationName);
	void set_pose_modifier(const std::string& boneName, const PoseModifier& modifier);
	void set_skeleton(const Skeleton_CPtr& skeleton);
	void update(int milliseconds);

	//#################### PRIVATE METHODS ####################
private:
	void reset_controller();
	void set_pose(const Pose_CPtr& pose);
	void update_pose(int milliseconds);
};

//#################### TYPEDEFS ####################
typedef shared_ptr<AnimationController> AnimationController_Ptr;
typedef shared_ptr<const AnimationController> AnimationController_CPtr;

}

#endif
