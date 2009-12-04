/***
 * hesperus: SoundSystem.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_SOUNDSYSTEM
#define H_HESP_SOUNDSYSTEM

#include <map>
#include <string>

#include <boost/function.hpp>
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
using boost::shared_ptr;
using boost::weak_ptr;

#include <fmod.hpp>

#include <hesp/util/IDAllocator.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class SoundInstance> SoundInstance_Ptr;
class SoundInstanceUpdater;
class SoundListenerUpdater;

//#################### HELPERS ####################
enum SoundFlags
{
	SF_CACHE	= 0,
	SF_STREAM	= 1,
	SF_2D		= 0,
	SF_3D		= 2,
	SF_NOLOOP	= 0,
	SF_LOOP		= 4,
};

class SoundSystem
{
	//#################### FORWARD DECLARATIONS ####################
private:
	struct InstanceData;

	//#################### TYPEDEFS ####################
private:
	typedef std::map<int,InstanceData> InstanceMap;
	typedef boost::function<void(SoundInstanceUpdater&)> InstanceUpdateFunc;
	typedef boost::function<void(SoundListenerUpdater&)> ListenerUpdateFunc;
	typedef std::map<std::string,FMOD::Sound*> SoundMap;

	//#################### NESTED CLASSES ####################
public:
	class InstanceHandle
	{
		friend class SoundSystem;

	private:
		weak_ptr<int> m_id;

		explicit InstanceHandle(const weak_ptr<int>& id)
		:	m_id(id)
		{}

	public:
		bool valid() const;
	};

private:
	struct InstanceData
	{
		shared_ptr<int> m_id;
		SoundInstance_Ptr m_instance;
		std::string m_name;
		boost::optional<InstanceUpdateFunc> m_updateFunc;

		InstanceData(const shared_ptr<int>& id, const SoundInstance_Ptr& instance,
					 const std::string& name, const boost::optional<InstanceUpdateFunc> updateFunc)
		:	m_id(id), m_instance(instance), m_name(name), m_updateFunc(updateFunc)
		{}
	};

	//#################### PRIVATE VARIABLES ####################
private:
	mutable IDAllocator m_idAllocator;
	mutable InstanceMap m_instances;
	boost::optional<ListenerUpdateFunc> m_listenerUpdateFunc;
	SoundMap m_sounds;
	FMOD::System *m_system;

	//#################### CONSTRUCTORS ####################
public:
	explicit SoundSystem(int channels = 100);

	//#################### COPY CONSTRUCTOR & ASSIGNMENT OPERATOR ####################
private:
	SoundSystem(const SoundSystem&);
	SoundSystem& operator=(const SoundSystem&);

	//#################### DESTRUCTOR ####################
public:
	~SoundSystem();

	//#################### PUBLIC METHODS ####################
public:
	void create_sound(const std::string& name, const std::string& filename, unsigned int flags, float minDistance = 1.0f, float maxDistance = 10000.0f);
	void destroy_all_instances();
	void destroy_all_instances(const std::string& name);
	void destroy_all_sounds();
	void destroy_instance(const InstanceHandle& handle);
	void destroy_sound(const std::string& name);
	bool has_sound(const std::string& name) const;
	bool is_playing(const InstanceHandle& handle) const;
	InstanceHandle play_sound(const std::string& name);
	InstanceHandle play_3d_sound(const std::string& name, const InstanceUpdateFunc& updateFunc);
	void set_3d_listener_update_func(const ListenerUpdateFunc& listenerUpdateFunc);
	void update();

	//#################### PRIVATE METHODS ####################
private:
	void check(FMOD_RESULT result);
	InstanceMap::iterator destroy_instance(InstanceMap::iterator it) const;
	InstanceHandle play_sound_sub(const std::string& name, const boost::optional<InstanceUpdateFunc>& updateFunc);
};

}

#endif
