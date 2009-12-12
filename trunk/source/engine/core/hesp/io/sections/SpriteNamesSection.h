/***
 * hesperus: SpriteNamesSection.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_SPRITENAMESSECTION
#define H_HESP_SPRITENAMESSECTION

#include <hesp/sprites/SpriteManager.h>
#include "ResourceNamesSection.h"

namespace hesp {

struct SpriteNamesSection : ResourceNamesSection<SpriteManager>
{
	//#################### CONSTRUCTORS ####################
	SpriteNamesSection();
};

}

#endif
