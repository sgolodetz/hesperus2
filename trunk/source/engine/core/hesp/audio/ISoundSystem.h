/***
 * hesperus: ISoundSystem.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ISOUNDSYSTEM
#define H_HESP_ISOUNDSYSTEM

#include <string>

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include "SoundInstanceHandle.h"

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

class ISoundSystem
{
	//#################### TYPEDEFS ####################
protected:
	typedef boost::function<void(SoundInstanceUpdater&)> InstanceUpdateFunc;
	typedef boost::function<void(SoundListenerUpdater&)> ListenerUpdateFunc;

	//#################### CONSTRUCTORS ####################
public:
	ISoundSystem() {}

	//#################### DESTRUCTOR ####################
public:
	virtual ~ISoundSystem() {}

	//#################### COPY CONSTRUCTOR & ASSIGNMENT OPERATOR ####################
private:
	ISoundSystem(const ISoundSystem&);
	ISoundSystem& operator=(const ISoundSystem&);

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void create_sound(const std::string& name, const std::string& filename, unsigned int flags, float minDistance = 1.0f, float maxDistance = 10000.0f) = 0;
	virtual void destroy_all_instances() = 0;
	virtual void destroy_all_instances(const std::string& name) = 0;
	virtual void destroy_all_sounds() = 0;
	virtual void destroy_instance(const SoundInstanceHandle& handle) = 0;
	virtual void destroy_sound(const std::string& name) = 0;
	virtual bool has_sound(const std::string& name) const = 0;
	virtual bool is_playing(const SoundInstanceHandle& handle) const = 0;
	virtual SoundInstanceHandle play_sound(const std::string& name) = 0;
	virtual SoundInstanceHandle play_3d_sound(const std::string& name, const InstanceUpdateFunc& updateFunc) = 0;
	virtual void set_3d_listener_update_func(const ListenerUpdateFunc& listenerUpdateFunc) = 0;
	virtual void update() = 0;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ISoundSystem> ISoundSystem_Ptr;

}

#endif
