/***
 * hesperus: SoundListenerUpdater.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_SOUNDLISTENERUPDATER
#define H_HESP_SOUNDLISTENERUPDATER

#include <fmod.hpp>

#include <hesp/math/vectors/Vector3.h>

namespace hesp {

class SoundListenerUpdater
{
	//#################### PRIVATE VARIABLES ####################
private:
	FMOD::System *m_system;

	//#################### CONSTRUCTORS ####################
public:
	explicit SoundListenerUpdater(FMOD::System *system);

	//#################### PUBLIC METHODS ####################
public:
	void set_3d_listener_attributes(const Vector3d& position, const Vector3d& velocity, const Vector3d& look, const Vector3d& up);
};

}

#endif
