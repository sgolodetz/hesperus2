/***
 * hesperus: NullSoundSystem.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_NULLSOUNDSYSTEM
#define H_HESP_NULLSOUNDSYSTEM

#include <set>
#include <string>

#include "ISoundSystem.h"

namespace hesp {

class NullSoundSystem : public ISoundSystem
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::set<std::string> m_sounds;

	//#################### PUBLIC METHODS ####################
public:
	void create_sound(const std::string& name, const std::string& filename, unsigned int flags, float minDistance = 1.0f, float maxDistance = 10000.0f);
	void destroy_all_instances();
	void destroy_all_instances(const std::string& name);
	void destroy_all_sounds();
	void destroy_instance(const SoundInstanceHandle& handle);
	void destroy_sound(const std::string& name);
	bool has_sound(const std::string& name) const;
	bool is_playing(const SoundInstanceHandle& handle) const;
	SoundInstanceHandle play_sound(const std::string& name);
	SoundInstanceHandle play_3d_sound(const std::string& name, const InstanceUpdateFunc& updateFunc);
	void set_3d_listener_update_func(const ListenerUpdateFunc& listenerUpdateFunc);
	void update();
};

}

#endif
