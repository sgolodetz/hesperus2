/***
 * hesperus: AnimationController.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "AnimationController.h"

#include <cmath>
#include <iostream>

#include "Animation.h"
#include "Pose.h"
#include "Skeleton.h"

namespace {

//#################### CONSTANTS ####################
enum
{
	TRANSITION_TIME = 50		// the desired length of an animation transition (in ms)
};

}

namespace hesp {

//#################### CONSTRUCTORS ####################
AnimationController::AnimationController(bool interpolateKeyframes)
:	m_interpolateKeyframes(interpolateKeyframes), m_state(AS_REST), m_animationName("<rest>"), m_animationTime(0)
{}

//#################### PUBLIC METHODS ####################
void AnimationController::clear_pose_modifiers()
{
	m_poseModifiers.clear();
}

const Pose_CPtr& AnimationController::get_pose() const
{
	return m_pose;
}

const std::map<std::string,PoseModifier>& AnimationController::get_pose_modifiers() const
{
	return m_poseModifiers;
}

void AnimationController::remove_pose_modifier(const std::string& boneName)
{
	m_poseModifiers.erase(boneName);
}

void AnimationController::request_animation(std::string newAnimationName)
{
	// If we're trying to request a non-existent animation, replace it with the rest animation.
	if(!m_skeleton->has_animation(newAnimationName))
	{
#if 0
		std::cerr << "Non-existent animation " << newAnimationName << ", defaulting to rest animation\n";
#endif
		newAnimationName = "<rest>";
	}

	// If we're already playing the requested animation, ignore the request.
	if(m_animationName == newAnimationName) return;

	// Set up a transition towards the new pose. Note that if we're already in a transition, we
	// keep transitioning, just towards the new target.
	if(m_state == AS_REST)
	{
		m_state = AS_TRANSITION;
		m_transitionStart = m_skeleton->make_rest_pose();
	}
	else if(m_state == AS_PLAY)
	{
		m_state = AS_TRANSITION;
		m_transitionStart = get_pose();
	}

	m_animationName = newAnimationName;
	m_animationTime = 0;
}

void AnimationController::set_pose_modifier(const std::string& boneName, const PoseModifier& modifier)
{
	remove_pose_modifier(boneName);
	m_poseModifiers.insert(std::make_pair(boneName, modifier));
}

void AnimationController::set_skeleton(const Skeleton_CPtr& skeleton)
{
	m_skeleton = skeleton;
	reset_controller();
}

void AnimationController::update(int milliseconds)
{
	update_pose(milliseconds);
}

//#################### PRIVATE METHODS ####################
void AnimationController::reset_controller()
{
	m_state = AS_REST;
	m_animationName = "<rest>";
	m_animationTime = 0;
	set_pose(m_skeleton->make_rest_pose());
	m_transitionStart.reset();
}

void AnimationController::set_pose(const Pose_CPtr& pose)
{
	m_pose = pose;
}

void AnimationController::update_pose(int milliseconds)
{
	switch(m_state)
	{
		case AS_REST:
		{
			set_pose(m_skeleton->make_rest_pose());
			break;
		}
		case AS_PLAY:
		{
			Animation_CPtr animation = m_skeleton->animation(m_animationName);
			int animationLength = static_cast<int>(animation->length() * 1000);

			m_animationTime += milliseconds;

			// Loop if we've gone past the end of the animation.
			if(animationLength > 0) m_animationTime %= animationLength;
			else m_animationTime = 0;

			// Calculate the keyframe index and interpolation parameter.
			int lastKeyframe = animation->keyframe_count() - 1;
			double animationFraction = animationLength > 0 ? (double)m_animationTime / animationLength : 0;
			double keyframePos = animationFraction * lastKeyframe;
			int keyframeIndex = static_cast<int>(ceil(keyframePos));
			double dummy;
			double t = modf(keyframePos, &dummy);	// t is the floating-point part of the keyframe position

			// Clamp the keyframe index to be safe.
			keyframeIndex = std::min(keyframeIndex, lastKeyframe);

			Pose_CPtr newPose = animation->keyframe(keyframeIndex);
			if(m_interpolateKeyframes)
			{
				int oldKeyframeIndex = (keyframeIndex + lastKeyframe) % animation->keyframe_count();
				Pose_CPtr oldPose = animation->keyframe(oldKeyframeIndex);
				set_pose(Pose::interpolate(oldPose, newPose, t));
			}
			else
			{
				set_pose(newPose);
			}
			break;
		}
		case AS_TRANSITION:
		{
			Pose_CPtr newPose;

			if(m_animationName != "<rest>")
			{
				// If the new animation isn't the rest animation, transition to its start keyframe.
				newPose = m_skeleton->animation(m_animationName)->keyframe(0);
			}
			else
			{
				newPose = m_skeleton->make_rest_pose();
			}

			m_animationTime += milliseconds;
			if(m_animationTime >= TRANSITION_TIME)
			{
				// The transition is over.
				m_state = m_animationName != "<rest>" ? AS_PLAY : AS_REST;
				m_animationTime = 0;
				m_transitionStart.reset();
				set_pose(newPose);
			}
			else
			{
				// Still transitioning: interpolate between the transition start pose and the new pose.
				double t = (double)m_animationTime / TRANSITION_TIME;
				set_pose(Pose::interpolate(m_transitionStart, newPose, t));
			}
			break;
		}
	}
}

}
