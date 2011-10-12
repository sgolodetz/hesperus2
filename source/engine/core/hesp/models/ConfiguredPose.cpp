/***
 * hesperus: ConfiguredPose.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ConfiguredPose.h"

#include <hesp/exceptions/Exception.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
ConfiguredPose::ConfiguredPose(const std::vector<ConfiguredBone_CPtr>& bones)
:	m_bones(bones)
{}

//#################### PUBLIC METHODS ####################
const ConfiguredBone_CPtr& ConfiguredPose::bones(int i) const
{
	if(0 <= i && i < static_cast<int>(m_bones.size()))
	{
		return m_bones[i];
	}
	else throw Exception("Bone index out of range");
}

}
