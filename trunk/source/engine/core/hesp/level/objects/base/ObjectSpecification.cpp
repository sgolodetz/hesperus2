/***
 * hesperus: ObjectSpecification.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ObjectSpecification.h"

#include <hesp/level/objects/components/CmpBasicModelRender.h>
#include <hesp/level/objects/components/CmpBasicProjectile.h>
#include <hesp/level/objects/components/CmpBipedAnimChooser.h>
#include <hesp/level/objects/components/CmpCharacterModelRender.h>
#include <hesp/level/objects/components/CmpConsumeActivatable.h>
#include <hesp/level/objects/components/CmpHealth.h>
#include <hesp/level/objects/components/CmpInventory.h>
#include <hesp/level/objects/components/CmpItemActivatable.h>
#include <hesp/level/objects/components/CmpMinimusScriptYoke.h>
#include <hesp/level/objects/components/CmpMovement.h>
#include <hesp/level/objects/components/CmpOrientation.h>
#include <hesp/level/objects/components/CmpOwnable.h>
#include <hesp/level/objects/components/CmpPosition.h>
#include <hesp/level/objects/components/CmpProjectileWeaponUsable.h>
#include <hesp/level/objects/components/CmpScriptedUsable.h>
#include <hesp/level/objects/components/CmpSimulation.h>
#include <hesp/level/objects/components/CmpSpriteRender.h>
#include <hesp/level/objects/components/CmpUserBipedYoke.h>
#include <hesp/util/Properties.h>

namespace hesp {

//#################### STATIC VARIABLES ####################
bool ObjectSpecification::s_mapsBuilt = false;
std::map<std::string,ObjectSpecification::ComponentLoader> ObjectSpecification::s_componentLoaders;
std::map<std::string,std::string> ObjectSpecification::s_groupNames;

//#################### PUBLIC METHODS ####################
void ObjectSpecification::add_component(const std::string& componentName, const Properties& properties)
{
	std::string groupName = lookup_group(componentName);
	bool succeeded = m_components.insert(std::make_pair(groupName, std::make_pair(componentName, properties))).second;
	if(!succeeded) throw Exception("Existing component with same group: " + componentName);
}

std::vector<IObjectComponent_Ptr> ObjectSpecification::instantiate_components() const
{
	std::vector<IObjectComponent_Ptr> ret;
	ret.reserve(m_components.size());

	for(ComponentMap::const_iterator it=m_components.begin(), iend=m_components.end(); it!=iend; ++it)
	{
		ret.push_back(invoke_component_loader(it->second.first, it->second.second));
	}

	return ret;
}

//#################### LOOKUP METHODS ####################
#define ADD_ELEMENTS(c) s_componentLoaders[#c] = &Cmp##c::load; s_groupNames[#c] = Cmp##c::static_group_type();

void ObjectSpecification::build_maps()
{
	if(!s_mapsBuilt)
	{
		ADD_ELEMENTS(BasicModelRender);
		ADD_ELEMENTS(BasicProjectile);
		ADD_ELEMENTS(BipedAnimChooser);
		ADD_ELEMENTS(CharacterModelRender);
		ADD_ELEMENTS(ConsumeActivatable);
		ADD_ELEMENTS(Health);
		ADD_ELEMENTS(Inventory);
		ADD_ELEMENTS(ItemActivatable);
		ADD_ELEMENTS(MinimusScriptYoke);
		ADD_ELEMENTS(Movement);
		ADD_ELEMENTS(Orientation);
		ADD_ELEMENTS(Ownable);
		ADD_ELEMENTS(Position);
		ADD_ELEMENTS(ProjectileWeaponUsable);
		ADD_ELEMENTS(ScriptedUsable);
		ADD_ELEMENTS(Simulation);
		ADD_ELEMENTS(SpriteRender);
		ADD_ELEMENTS(UserBipedYoke);
		s_mapsBuilt = true;
	}
}

#undef ADD_ELEMENTS

std::map<std::string,ObjectSpecification::ComponentLoader>& ObjectSpecification::component_loaders()
{
	build_maps();
	return s_componentLoaders;
}

std::map<std::string,std::string>& ObjectSpecification::group_names()
{
	build_maps();
	return s_groupNames;
}

IObjectComponent_Ptr ObjectSpecification::invoke_component_loader(const std::string& componentName, const Properties& properties)
{
	std::map<std::string,ComponentLoader>& loaders = component_loaders();
	std::map<std::string,ComponentLoader>::iterator it = loaders.find(componentName);
	if(it != loaders.end()) return (*(it->second))(properties);
	else throw Exception("No loader registered for components of type " + componentName);
}

std::string ObjectSpecification::lookup_group(const std::string& componentName)
{
	std::map<std::string,std::string>& groupNames = group_names();
	std::map<std::string,std::string>::const_iterator it = groupNames.find(componentName);
	if(it != groupNames.end()) return it->second;
	else throw Exception("No group specified for components of type: " + componentName);
}

}
