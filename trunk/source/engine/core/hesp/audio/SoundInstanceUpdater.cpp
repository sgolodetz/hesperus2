/***
 * hesperus: SoundInstanceUpdater.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "SoundInstanceUpdater.h"

#include "SoundInstance.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
SoundInstanceUpdater::SoundInstanceUpdater(SoundInstance& instance)
:	m_instance(instance)
{}

//#################### PUBLIC METHODS ####################
void SoundInstanceUpdater::set_3d_attributes(const Vector3d& position, const Vector3d& velocity)
{
	m_instance.set_3d_attributes(position, velocity);
}

}
