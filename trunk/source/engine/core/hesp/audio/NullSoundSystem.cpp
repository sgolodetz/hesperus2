/***
 * hesperus: NullSoundSystem.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "NullSoundSystem.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
void NullSoundSystem::create_sound(const std::string& name, const std::string& filename, unsigned int flags,
								   float minDistance, float maxDistance)
{
	m_sounds.insert(name);
}

void NullSoundSystem::destroy_all_instances()
{}

void NullSoundSystem::destroy_all_instances(const std::string& name)
{}

void NullSoundSystem::destroy_all_sounds()
{
	m_sounds.clear();
}

void NullSoundSystem::destroy_instance(const SoundInstanceHandle& handle)
{}

void NullSoundSystem::destroy_sound(const std::string& name)
{
	m_sounds.erase(name);
}

bool NullSoundSystem::has_sound(const std::string& name) const
{
	return m_sounds.find(name) != m_sounds.end();
}

bool NullSoundSystem::is_playing(const SoundInstanceHandle& handle) const
{
	return false;
}

SoundInstanceHandle NullSoundSystem::play_sound(const std::string& name)
{
	return SoundInstanceHandle(weak_ptr<int>());
}

SoundInstanceHandle NullSoundSystem::play_3d_sound(const std::string& name, const InstanceUpdateFunc& updateFunc)
{
	return SoundInstanceHandle(weak_ptr<int>());
}

void NullSoundSystem::set_3d_listener_update_func(const ListenerUpdateFunc& listenerUpdateFunc)
{}

void NullSoundSystem::update()
{}

}
