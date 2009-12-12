/***
 * hesperus: CmpMinimusScriptYoke.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpMinimusScriptYoke.h"

#include <hesp/level/objects/components/ICmpModelRender.h>
#include <hesp/level/objects/components/ICmpMovement.h>
#include <hesp/level/objects/components/ICmpOrientation.h>
#include <hesp/level/objects/components/ICmpSimulation.h>
#include <hesp/level/objects/yokes/minimus/MinimusScriptYoke.h>
#include <hesp/util/Properties.h>
namespace bf = boost::filesystem;

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpMinimusScriptYoke::CmpMinimusScriptYoke(const std::string& scriptName)
:	m_scriptName(scriptName)
{}

//#################### STATIC FACTORY METHODS ####################
IObjectComponent_Ptr CmpMinimusScriptYoke::load(const Properties& properties)
{
	return IObjectComponent_Ptr(new CmpMinimusScriptYoke(properties.get<std::string>("Script")));
}

//#################### PUBLIC METHODS ####################
void CmpMinimusScriptYoke::check_dependencies() const
{
	check_dependency<ICmpModelRender>();
	check_dependency<ICmpMovement>();
	check_dependency<ICmpOrientation>();
	check_dependency<ICmpSimulation>();
}

std::vector<ObjectCommand_Ptr> CmpMinimusScriptYoke::generate_commands(InputState& input, const std::vector<CollisionPolygon_Ptr>& polygons,
																	   const OnionTree_CPtr& tree, const NavManager_CPtr& navManager)
{
	if(!m_yoke) m_yoke.reset(new MinimusScriptYoke(m_objectID, m_objectManager, m_scriptName, m_objectManager->ai_engine()));
	return m_yoke->generate_commands(input, polygons, tree, navManager);
}

Properties CmpMinimusScriptYoke::save() const
{
	Properties properties;
	properties.set("Script", m_scriptName);
	return properties;
}

}
