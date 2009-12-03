/***
 * hesperus: ContactResolverRegistry.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CONTACTRESOLVERREGISTRY
#define H_HESP_CONTACTRESOLVERREGISTRY

#include <map>
#include <utility>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include "PhysicsMaterial.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<const class ContactResolver> ContactResolver_CPtr;

class ContactResolverRegistry
{
	//#################### TYPEDEFS ####################
private:
	typedef std::map<std::pair<PhysicsMaterial,PhysicsMaterial>,ContactResolver_CPtr> ResolverMap;

	//#################### PRIVATE VARIABLES ####################
private:
	ResolverMap m_resolvers;

	//#################### PUBLIC METHODS ####################
public:
	ContactResolver_CPtr lookup_resolver(PhysicsMaterial material1, PhysicsMaterial material2) const;
	void remove_resolver(PhysicsMaterial material1, PhysicsMaterial material2);
	void set_resolver(PhysicsMaterial material1, PhysicsMaterial material2, const ContactResolver_CPtr& resolver);

	//#################### PRIVATE METHODS ####################
private:
	static std::pair<PhysicsMaterial,PhysicsMaterial> make_material_pair(PhysicsMaterial material1, PhysicsMaterial material2);
};

}

#endif
