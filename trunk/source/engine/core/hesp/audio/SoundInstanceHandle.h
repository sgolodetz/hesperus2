/***
 * hesperus: SoundInstanceHandle.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_SOUNDINSTANCEHANDLE
#define H_HESP_SOUNDINSTANCEHANDLE

#include <boost/weak_ptr.hpp>
using boost::weak_ptr;

namespace hesp {

class SoundInstanceHandle
{
	//#################### FRIENDS ####################
	friend class NullSoundSystem;
	friend class SoundSystem;

	//#################### PRIVATE VARIABLES ####################
private:
	weak_ptr<int> m_id;

	//#################### CONSTRUCTORS ####################
private:
	explicit SoundInstanceHandle(const weak_ptr<int>& id)
	:	m_id(id)
	{}
};

}

#endif
