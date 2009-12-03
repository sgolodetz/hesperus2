/***
 * hesperus: ModelNamesSection.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_MODELNAMESSECTION
#define H_HESP_MODELNAMESSECTION

#include <hesp/level/models/ModelManager.h>
#include "ResourceNamesSection.h"

namespace hesp {

struct ModelNamesSection : ResourceNamesSection<ModelManager>
{
	//#################### CONSTRUCTORS ####################
	ModelNamesSection();
};

}

#endif
