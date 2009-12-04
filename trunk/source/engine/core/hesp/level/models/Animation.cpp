/***
 * hesperus: Animation.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Animation.h"

#include <boost/lexical_cast.hpp>
using boost::bad_lexical_cast;
using boost::lexical_cast;

#include <hesp/exceptions/Exception.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
Animation::Animation(double length, const std::vector<Pose_CPtr>& keyframes)
:	m_length(length), m_keyframes(keyframes)
{}

//#################### PUBLIC METHODS ####################
int Animation::keyframe_count() const
{
	return static_cast<int>(m_keyframes.size());
}

Pose_CPtr Animation::keyframe(int i) const
{
	if(0 <= i && i < static_cast<int>(m_keyframes.size()) && m_keyframes[i]) return m_keyframes[i];
	else throw Exception("Invalid keyframe " + lexical_cast<std::string>(i));
}

double Animation::length() const
{
	return m_length;
}

}
