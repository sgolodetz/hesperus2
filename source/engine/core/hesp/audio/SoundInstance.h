/***
 * hesperus: SoundInstance.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_SOUNDINSTANCE
#define H_HESP_SOUNDINSTANCE

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <fmod.hpp>

#include <hesp/math/vectors/Vector3.h>

namespace hesp {

class SoundInstance
{
	//#################### PRIVATE VARIABLES ####################
private:
	FMOD::Channel *m_channel;

	//#################### CONSTRUCTORS ####################
public:
	explicit SoundInstance(FMOD::Channel *channel);

	//#################### DESTRUCTOR ####################
public:
	~SoundInstance();

	//#################### COPY CONSTRUCTOR & ASSIGNMENT OPERATOR ####################
private:
	SoundInstance(const SoundInstance&);
	SoundInstance& operator=(const SoundInstance&);

	//#################### PUBLIC METHODS ####################
public:
	bool is_playing() const;
	void set_3d_attributes(const Vector3d& position, const Vector3d& velocity);
	void stop();

	//#################### PRIVATE METHODS ####################
private:
	void check(FMOD_RESULT result);
};

//#################### TYPEDEFS ####################
typedef shared_ptr<SoundInstance> SoundInstance_Ptr;

}

#endif
