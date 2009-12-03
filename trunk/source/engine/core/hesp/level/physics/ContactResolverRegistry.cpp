/***
 * hesperus: ContactResolverRegistry.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ContactResolverRegistry.h"

namespace hesp {

//#################### PUBLIC METHODS ####################
ContactResolver_CPtr ContactResolverRegistry::lookup_resolver(PhysicsMaterial material1, PhysicsMaterial material2) const
{
	ResolverMap::const_iterator it = m_resolvers.find(make_material_pair(material1, material2));
	if(it != m_resolvers.end()) return it->second;
	else return ContactResolver_CPtr();
}

void ContactResolverRegistry::remove_resolver(PhysicsMaterial material1, PhysicsMaterial material2)
{
	m_resolvers.erase(make_material_pair(material1, material2));
}

void ContactResolverRegistry::set_resolver(PhysicsMaterial material1, PhysicsMaterial material2,
										   const ContactResolver_CPtr& resolver)
{
	m_resolvers[make_material_pair(material1, material2)] = resolver;
}

//#################### PRIVATE METHODS ####################
std::pair<PhysicsMaterial,PhysicsMaterial>
ContactResolverRegistry::make_material_pair(PhysicsMaterial material1, PhysicsMaterial material2)
{
	if(material1 < material2)	return std::make_pair(material1, material2);
	else						return std::make_pair(material2, material1);
}

}
