/***
 * hesperus: CmpUserBipedYoke.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpUserBipedYoke.h"

#include <hesp/exceptions/Exception.h>
#include <hesp/objects/components/ICmpMovement.h>
#include <hesp/objects/components/ICmpOrientation.h>
#include <hesp/objects/components/ICmpSimulation.h>
#include <hesp/objects/yokes/user/UserBipedYoke.h>
#include <hesp/util/Properties.h>

namespace hesp {

//#################### STATIC FACTORY METHODS ####################
IObjectComponent_Ptr CmpUserBipedYoke::load(const Properties&)
{
	return IObjectComponent_Ptr(new CmpUserBipedYoke);
}

//#################### PUBLIC METHODS ####################
void CmpUserBipedYoke::check_dependencies() const
{
	check_dependency<ICmpMovement>();
	check_dependency<ICmpOrientation>();
	check_dependency<ICmpSimulation>();
}

std::vector<ObjectCommand_Ptr> CmpUserBipedYoke::generate_commands(InputState& input)
{
	if(!m_yoke) m_yoke.reset(new UserBipedYoke(m_objectID, m_objectManager));
	return m_yoke->generate_commands(input);
}

Properties CmpUserBipedYoke::save() const
{
	return Properties();
}

}
