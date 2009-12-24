/***
 * hesperus: AiScriptRegistrar.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "AiScriptRegistrar.h"

#include <hesp/objects/yokes/minimus/MinimusScriptYoke.h>

namespace hesp {

//#################### PUBLIC METHODS ####################
void AiScriptRegistrar::run(const ASXEngine_Ptr& engine)
{
	MinimusScriptYoke::register_for_scripting(engine);
}

}
