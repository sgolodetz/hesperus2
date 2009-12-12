/***
 * hesperus: ObjectComponent.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <hesp/exceptions/Exception.h>
#include "ObjectManager.h"

namespace hesp {

//#################### PROTECTED METHODS ####################
template <typename C>
void ObjectComponent::check_dependency() const
{
	if(!m_objectManager->get_component<C>(m_objectID))
	{
		throw Exception(own_type() + " (ObjectID " + m_objectID.to_string() + "): Requires " + C::static_own_type() + " component");
	}
}

}
