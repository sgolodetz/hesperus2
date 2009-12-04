/***
 * hesperus: SoundInstanceUpdater.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_SOUNDINSTANCEUPDATER
#define H_HESP_SOUNDINSTANCEUPDATER

#include <hesp/math/vectors/Vector3.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
class SoundInstance;

class SoundInstanceUpdater
{
	//#################### PRIVATE VARIABLES ####################
private:
	SoundInstance& m_instance;

	//#################### CONSTRUCTORS ####################
public:
	SoundInstanceUpdater(SoundInstance& instance);

	//#################### PUBLIC METHODS ####################
public:
	void set_3d_attributes(const Vector3d& position, const Vector3d& velocity);
};

}

#endif
