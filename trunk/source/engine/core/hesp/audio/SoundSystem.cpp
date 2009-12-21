/***
 * hesperus: SoundSystem.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "SoundSystem.h"

#include <fmod_errors.h>

#include <hesp/exceptions/Exception.h>
#include "SoundInstance.h"
#include "SoundInstanceUpdater.h"
#include "SoundListenerUpdater.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
SoundSystem::SoundSystem(int channels)
{
	// Create the system.
	FMOD_RESULT result = FMOD::System_Create(&m_system);
	check(result);

	// Initialise the system.
	result = m_system->init(channels, FMOD_INIT_3D_RIGHTHANDED, NULL);
	check(result);
}

//#################### DESTRUCTOR ####################
SoundSystem::~SoundSystem()
{
	// Destroy all the sounds.
	destroy_all_sounds();

	// Release the system.
	FMOD_RESULT result = m_system->release();
	check(result);
}

//#################### PUBLIC METHODS ####################
/**
Creates a sound with the specified name.

@param name			The name of the sound
@param filename		The name of the file from which to load the sound
@param flags		Flags affecting the sound creation (e.g. cached/streamed, 2D/3D, non-looping/looping)
@param minDistance	The distance from the source at which the sound should start to attenuate
@param maxDistance	The distance from the source at which the sound should stop attenuating
*/
void SoundSystem::create_sound(const std::string& name, const std::string& filename, unsigned int flags,
							   float minDistance, float maxDistance)
{
	if(has_sound(name))
	{
		throw Exception("A sound named " + name + " already exists");
	}

	FMOD_MODE mode = 0;
	if(flags & SF_STREAM) mode |= FMOD_CREATESTREAM;
	mode |= (flags & SF_3D) ? FMOD_3D : FMOD_2D;
	mode |= (flags & SF_LOOP) ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
	mode |= FMOD_HARDWARE;

	FMOD::Sound *sound;
	FMOD_RESULT result = m_system->createSound(filename.c_str(), mode, NULL, &sound);
	check(result);

	result = sound->set3DMinMaxDistance(minDistance, maxDistance);
	check(result);

	m_sounds.insert(std::make_pair(name, sound));
}

/**
Destroys every sound instance (of every sound in the system).
*/
void SoundSystem::destroy_all_instances()
{
	m_instances.clear();
	m_idAllocator.reset();
}

/**
Destroys every instance of the sound with the specified name.

@param name		The name of the sound
*/
void SoundSystem::destroy_all_instances(const std::string& name)
{
	for(InstanceMap::iterator it=m_instances.begin(), iend=m_instances.end(); it!=iend;)
	{
		if(it->second.m_name == name) it = destroy_instance(it);
		else ++it;
	}
}

/**
Destroys all the sounds stored in the system (this includes destroying
any instances of them which might be playing).
*/
void SoundSystem::destroy_all_sounds()
{
	destroy_all_instances();

	FMOD_RESULT result;
	for(SoundMap::iterator it=m_sounds.begin(), iend=m_sounds.end(); it!=iend; ++it)
	{
		result = it->second->release();
		check(result);
	}
	m_sounds.clear();
}

/**
Destroy the specified sound instance (this has the effect of stopping the sound).

@param handle	A handle to the instance
*/
void SoundSystem::destroy_instance(const SoundInstanceHandle& handle)
{
	shared_ptr<int> id = handle.m_id.lock();
	if(id) destroy_instance(m_instances.find(*id));		// note that the lock only succeeds if *id is in the map
}

/**
Destroys the named sound (this includes destroying any instances of it
which might be playing).

@param name		The name of the sound to destroy
*/
void SoundSystem::destroy_sound(const std::string& name)
{
	SoundMap::iterator it = m_sounds.find(name);
	if(it == m_sounds.end()) throw Exception("No such sound: " + name);

	destroy_all_instances(name);

	FMOD_RESULT result = it->second->release();
	m_sounds.erase(it);
	check(result);
}

/**
Returns whether or not the system contains a sound with the given name.

@param name		The name of the sound for which to test
@return			true, if the system contains a sound with the given name, or false otherwise
*/
bool SoundSystem::has_sound(const std::string& name) const
{
	return m_sounds.find(name) != m_sounds.end();
}

/**
Returns whether or not the specified instance handle refers to an instance
which is currently playing.

@param handle	A handle to the instance
@return			true, if the handle refers to a playing instance, or false otherwise
*/
bool SoundSystem::is_playing(const SoundInstanceHandle& handle) const
{
	shared_ptr<int> id = handle.m_id.lock();
	if(!id) return false;

	InstanceMap::iterator it = m_instances.find(*id);
	assert(it != m_instances.end());	// if the lock succeeds, then *id must be in the map

	bool isPlaying = it->second.m_instance->is_playing();
	if(!isPlaying) destroy_instance(it);
	return isPlaying;
}

/**
Plays the sound with the specified name.

@param name		The name of the sound
@return			A handle to the instance of the played sound
*/
SoundInstanceHandle SoundSystem::play_sound(const std::string& name)
{
	return play_sound_sub(name, boost::none);
}

/**
Plays the 3D sound with the specified name.

@param name			The name of the sound
@param updateFunc	The function used to update the sound's position and velocity each frame
@return				A handle to the instance of the played sound
*/
SoundInstanceHandle SoundSystem::play_3d_sound(const std::string& name, const InstanceUpdateFunc& updateFunc)
{
	return play_sound_sub(name, updateFunc);
}

/**
Sets the update function for the 3D listener.

@param listenerUpdateFunc	The function used to update the listener's position etc. each frame.
*/
void SoundSystem::set_3d_listener_update_func(const ListenerUpdateFunc& listenerUpdateFunc)
{
	m_listenerUpdateFunc = listenerUpdateFunc;
}

/**
Updates the sound system (should be called once each frame).
*/
void SoundSystem::update()
{
	if(m_listenerUpdateFunc)
	{
		SoundListenerUpdater updater(m_system);
		(*m_listenerUpdateFunc)(updater);
	}

	for(InstanceMap::iterator it=m_instances.begin(), iend=m_instances.end(); it!=iend;)
	{
		InstanceData& data = it->second;
		if(data.m_instance->is_playing())
		{
			if(data.m_updateFunc)
			{
				SoundInstanceUpdater updater(*data.m_instance);
				(*data.m_updateFunc)(updater);
			}
			++it;
		}
		else it = destroy_instance(it);
	}

	m_system->update();
}

//#################### PRIVATE METHODS ####################
void SoundSystem::check(FMOD_RESULT result)
{
	if(result != FMOD_OK)
	{
		throw Exception(FMOD_ErrorString(result));
	}
}

SoundSystem::InstanceMap::iterator SoundSystem::destroy_instance(InstanceMap::iterator it) const
{
	int id = it->first;
	m_instances.erase(it++);
	m_idAllocator.deallocate(id);
	return it;
}

SoundInstanceHandle
SoundSystem::play_sound_sub(const std::string& name, const boost::optional<InstanceUpdateFunc>& updateFunc)
{
	SoundMap::iterator it = m_sounds.find(name);
	if(it == m_sounds.end()) throw Exception("No such sound: " + name);

	FMOD::Sound *sound = it->second;
	FMOD::Channel *channel;
	FMOD_RESULT result = m_system->playSound(FMOD_CHANNEL_FREE, sound, true, &channel);
	check(result);

	SoundInstance_Ptr instance(new SoundInstance(channel));
	if(updateFunc)
	{
		SoundInstanceUpdater updater(*instance);
		(*updateFunc)(updater);
	}
	shared_ptr<int> id(new int(m_idAllocator.allocate()));
	m_instances.insert(std::make_pair(*id, InstanceData(id, instance, name, updateFunc)));

	result = channel->setPaused(false);
	check(result);

	return SoundInstanceHandle(weak_ptr<int>(id));
}

}
