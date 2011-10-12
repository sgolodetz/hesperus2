/***
 * hesperus: ContactResolver.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ContactResolver.h"

#include <hesp/physics/Contact.h>

namespace hesp {

//#################### PUBLIC METHODS ####################
void ContactResolver::resolve_contact(const Contact& contact, const OnionTree_CPtr& tree) const
{
	if(contact.objectB())	resolve_object_object(contact, tree);
	else					resolve_object_world(contact);
}

}
