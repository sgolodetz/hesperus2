/***
 * hesperus: SoundInstance.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "SoundInstance.h"

#include <fmod_errors.h>

#include <hesp/exceptions/Exception.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
SoundInstance::SoundInstance(FMOD::Channel *channel)
:	m_channel(channel)
{}

//#################### DESTRUCTOR ####################
SoundInstance::~SoundInstance()
{
	m_channel->stop();
}

//#################### PUBLIC METHODS ####################
bool SoundInstance::is_playing() const
{
	bool isPlaying;
	FMOD_RESULT result = m_channel->isPlaying(&isPlaying);
	if(result != FMOD_OK) return false;
	return isPlaying;
}

void SoundInstance::set_3d_attributes(const Vector3d& position, const Vector3d& velocity)
{
	FMOD_VECTOR P = {(float)position.x, (float)position.y, (float)position.z};
	FMOD_VECTOR V = {(float)velocity.x, (float)velocity.y, (float)velocity.z};
	FMOD_RESULT result = m_channel->set3DAttributes(&P, &V);
	check(result);
}

void SoundInstance::stop()
{
	FMOD_RESULT result = m_channel->stop();
	check(result);
}

//#################### PRIVATE METHODS ####################
void SoundInstance::check(FMOD_RESULT result)
{
	if(result != FMOD_OK)
	{
		throw Exception(FMOD_ErrorString(result));
	}
}

}
