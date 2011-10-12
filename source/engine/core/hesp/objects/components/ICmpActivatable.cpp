/***
 * hesperus: ICmpActivatable.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ICmpActivatable.h"

#include "ICmpModelRender.h"
#include "ICmpSimulation.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
void ICmpActivatable::check_dependencies() const
{
	check_dependency<ICmpModelRender>();
	check_dependency<ICmpSimulation>();
}

}
