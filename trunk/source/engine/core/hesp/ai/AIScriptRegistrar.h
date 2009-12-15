/***
 * hesperus: AIScriptRegistrar.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_AISCRIPTREGISTRAR
#define H_HESP_AISCRIPTREGISTRAR

#include <ASXEngine.h>

namespace hesp {

struct AIScriptRegistrar
{
	//#################### PUBLIC METHODS ####################
	static void run(const ASXEngine_Ptr& engine);
};

}

#endif
