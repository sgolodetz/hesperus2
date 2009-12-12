/***
 * hesperus: CmpConsumeActivatable.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpConsumeActivatable.h"

#include <hesp/util/Properties.h>
#include "ICmpInventory.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpConsumeActivatable::CmpConsumeActivatable(const std::string& type, int amount)
:	m_type(type), m_amount(amount)
{}

//#################### STATIC FACTORY METHODS ####################
IObjectComponent_Ptr CmpConsumeActivatable::load(const Properties& properties)
{
	return IObjectComponent_Ptr(new CmpConsumeActivatable(properties.get<std::string>("Type"), properties.get<int>("Amount")));
}

//#################### PUBLIC METHODS ####################
void CmpConsumeActivatable::activated_by(const ObjectID& activator)
{
	ICmpInventory_Ptr cmpActivatorInventory = m_objectManager->get_component(activator, cmpActivatorInventory);

	// If the activating object (e.g. the player) has no inventory, the item can't be consumed.
	if(!cmpActivatorInventory) return;

	// Add the consumables to the activating object's inventory.
	cmpActivatorInventory->add_consumables(m_type, m_amount);

	// Queue the consumed item (e.g. the ammo box) for destruction.
	m_objectManager->queue_for_destruction(m_objectID);
}

Properties CmpConsumeActivatable::save() const
{
	Properties properties;
	properties.set("Type", m_type);
	properties.set("Amount", m_amount);
	return properties;
}

}
