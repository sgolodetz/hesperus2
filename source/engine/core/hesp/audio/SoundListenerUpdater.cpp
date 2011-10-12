/***
 * hesperus: SoundListenerUpdater.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "SoundListenerUpdater.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
SoundListenerUpdater::SoundListenerUpdater(FMOD::System *system)
:	m_system(system)
{}

//#################### PUBLIC METHODS ####################
void SoundListenerUpdater::set_3d_listener_attributes(const Vector3d& position, const Vector3d& velocity,
													  const Vector3d& look, const Vector3d& up)
{
	FMOD_VECTOR P = {(float)position.x, (float)position.y, (float)position.z};
	FMOD_VECTOR V = {(float)velocity.x, (float)velocity.y, (float)velocity.z};
	FMOD_VECTOR L = {(float)look.x, (float)look.y, (float)look.z};
	FMOD_VECTOR U = {(float)up.x, (float)up.y, (float)up.z};
	m_system->set3DListenerAttributes(0, &P, &V, &L, &U);
}

}
