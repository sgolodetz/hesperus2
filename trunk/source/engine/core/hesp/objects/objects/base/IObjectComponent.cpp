/***
 * hesperus: IObjectComponent.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "IObjectComponent.h"

namespace hesp {

//#################### DESTRUCTOR ####################
IObjectComponent::~IObjectComponent() {}

//#################### PUBLIC METHODS ####################
void IObjectComponent::check_dependencies() const
{
	// Note:	A default implementation is provided so that components which don't have any dependencies
	//			to check aren't required to provide a stub implementation.
}

const ObjectID& IObjectComponent::object_id() const
{
	return m_objectID;
}

void IObjectComponent::register_listening()
{
	// Note:	A default implementation is provided so that components which don't need to listen
	//			to any messages aren't required to provide a stub implementation.
}

//#################### PROTECTED METHODS ####################
void IObjectComponent::set_object_id(const ObjectID& objectID)
{
	m_objectID = objectID;
}

void IObjectComponent::set_object_manager(ObjectManager *objectManager)
{
	m_objectManager = objectManager;
}

}
